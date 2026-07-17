// Ymir Web bridge — exposes the ymir-core emulator to JavaScript via Embind.
//
// Data flow (all memory-based, no filesystem dependency except MEMFS reads):
//   JS  --(BIOS bytes)-->  LoadIPL(span)
//   JS  --(files into MEMFS)-->  media::LoadDisc(path, preloadToRAM=true)
//   emu --(video callback)-->  framebuffer vector  -->  JS typed view --> Canvas
//   emu --(audio callback)-->  S16 stereo ring buf -->  JS pulls --> WebAudio
//   JS  --(button mask)-->  pad state  <--(pull model)-- peripheral report cb
//   emu <--(backup RAM ReadAll/WriteByte)-->  JS <--> IndexedDB

#include <ymir/ymir.hpp>

#include <ymir/sys/backup_ram.hpp>

#include <emscripten/bind.h>
#include <emscripten/val.h>

#include <algorithm>
#include <array>
#include <cstdint>
#include <memory>
#include <string>
#include <vector>

namespace {

struct WebEmuState {
    std::unique_ptr<ymir::Saturn> saturn;

    // Video output (written from the emulator's frame-complete callback)
    std::vector<uint32> framebuffer;
    uint32 fbWidth = 0;
    uint32 fbHeight = 0;

    // Audio ring buffer: interleaved S16 stereo samples.
    static constexpr size_t kAudioCapacity = 1 << 17; // int16 elements (65536 stereo frames)
    std::array<int16_t, kAudioCapacity> audioBuf{};
    size_t audioReadPos = 0;
    size_t audioWritePos = 0;
    std::vector<int16_t> audioOut; // staging buffer for JS reads

    // Input state: bitmask of ymir::peripheral::Button, 1=released / 0=pressed.
    uint16 padState[2] = {0xFFFF, 0xFFFF};
};

WebEmuState g_emu;

// ---------------------------------------------------------------------------
// Emulator callbacks (C-style: function pointer + void* context)
// ---------------------------------------------------------------------------

void OnVideoFrame(uint32 *fb, uint32 width, uint32 height, void *) {
    const size_t count = static_cast<size_t>(width) * height;
    g_emu.framebuffer.assign(fb, fb + count);
    g_emu.fbWidth = width;
    g_emu.fbHeight = height;
}

void OnAudioSample(sint16 left, sint16 right, void *) {
    const size_t next = (g_emu.audioWritePos + 2) % WebEmuState::kAudioCapacity;
    if (next == g_emu.audioReadPos) {
        // Buffer full: drop the oldest stereo frame to bound latency.
        g_emu.audioReadPos = (g_emu.audioReadPos + 2) % WebEmuState::kAudioCapacity;
    }
    g_emu.audioBuf[g_emu.audioWritePos] = left;
    g_emu.audioBuf[(g_emu.audioWritePos + 1) % WebEmuState::kAudioCapacity] = right;
    g_emu.audioWritePos = next;
}

template <int Port>
void OnPadReport(ymir::peripheral::PeripheralReport &report, void *) {
    report.report.controlPad.buttons = static_cast<ymir::peripheral::Button>(g_emu.padState[Port]);
}

// ---------------------------------------------------------------------------
// Embind-exposed API
// ---------------------------------------------------------------------------

bool init() {
    if (g_emu.saturn) {
        return true;
    }
    try {
        g_emu.saturn = std::make_unique<ymir::Saturn>();
    } catch (const std::exception &e) {
        printf("[ymir-web] Saturn construction failed: %s\n", e.what());
        return false;
    } catch (...) {
        printf("[ymir-web] Saturn construction failed: unknown exception\n");
        return false;
    }
    auto &saturn = *g_emu.saturn;

    // Single-threaded rendering: the wasm build does not use pthreads.
    saturn.configuration.video.threadedVDP1 = false;
    saturn.configuration.video.threadedDeinterlacer = false;
    saturn.configuration.video.threadedVDP2 = false;

    // In-memory internal backup RAM (32 KiB); persistence is handled by JS/IndexedDB.
    ymir::bup::BackupMemory bupMem;
    bupMem.CreateInMemory(ymir::bup::BackupMemorySize::_256Kbit);
    saturn.mem.SetInternalBackupRAM(std::move(bupMem));

    // Output callbacks.
    saturn.VDP.SetSoftwareRenderCallback({nullptr, &OnVideoFrame});
    saturn.SCSP.SetSampleCallback({nullptr, &OnAudioSample});

    // Input: digital control pad on both ports, pull-model report callbacks.
    saturn.SMPC.GetPeripheralPort1().SetPeripheralReportCallback({nullptr, &OnPadReport<0>});
    saturn.SMPC.GetPeripheralPort1().ConnectControlPad();
    saturn.SMPC.GetPeripheralPort2().SetPeripheralReportCallback({nullptr, &OnPadReport<1>});
    saturn.SMPC.GetPeripheralPort2().ConnectControlPad();
    return true;
}

std::string getVersion() {
    return Ymir_VERSION;
}

bool loadIPL(emscripten::val data) {
    if (!g_emu.saturn) {
        return false;
    }
    const std::vector<uint8> buf = emscripten::convertJSArrayToNumberVector<uint8>(data);
    if (buf.size() != ymir::sys::kIPLSize) {
        return false;
    }
    std::array<uint8, ymir::sys::kIPLSize> ipl;
    std::copy(buf.begin(), buf.end(), ipl.begin());
    g_emu.saturn->LoadIPL(ipl);
    return true;
}

// Loads a disc image already written to MEMFS by JS (e.g. /roms/game.cue or /roms/game.chd).
bool loadDisc(std::string path) {
    if (!g_emu.saturn) {
        return false;
    }
    ymir::media::Disc disc;
    auto cbMsg = [](ymir::media::MessageType, std::string) {};
    if (!ymir::media::LoadDisc(path, disc, /*preloadToRAM=*/true, cbMsg)) {
        return false;
    }
    g_emu.saturn->LoadDisc(std::move(disc));
    g_emu.saturn->AutodetectRegion();
    return true;
}

void ejectDisc() {
    if (g_emu.saturn) {
        g_emu.saturn->EjectDisc();
    }
}

void reset(bool hard) {
    if (g_emu.saturn) {
        g_emu.saturn->Reset(hard);
    }
}

void runFrame() {
    if (g_emu.saturn) {
        g_emu.saturn->RunFrame();
    }
}

// --- Video -----------------------------------------------------------------

emscripten::val getFramebuffer() {
    return emscripten::val(emscripten::typed_memory_view(
        g_emu.framebuffer.size() * sizeof(uint32), reinterpret_cast<const uint8 *>(g_emu.framebuffer.data())));
}
uint32 getFrameWidth() { return g_emu.fbWidth; }
uint32 getFrameHeight() { return g_emu.fbHeight; }

// --- Audio -----------------------------------------------------------------

// Number of stereo frames currently buffered.
size_t audioAvailable() {
    const size_t elems =
        (g_emu.audioWritePos + WebEmuState::kAudioCapacity - g_emu.audioReadPos) % WebEmuState::kAudioCapacity;
    return elems / 2;
}

// Drains up to maxFrames stereo frames; returns an Int16Array view (interleaved L,R).
emscripten::val readAudio(size_t maxFrames) {
    const size_t frames = std::min(maxFrames, audioAvailable());
    g_emu.audioOut.resize(frames * 2);
    for (size_t i = 0; i < frames * 2; ++i) {
        g_emu.audioOut[i] = g_emu.audioBuf[(g_emu.audioReadPos + i) % WebEmuState::kAudioCapacity];
    }
    g_emu.audioReadPos = (g_emu.audioReadPos + frames * 2) % WebEmuState::kAudioCapacity;
    return emscripten::val(emscripten::typed_memory_view(g_emu.audioOut.size(), g_emu.audioOut.data()));
}

// --- Input -----------------------------------------------------------------

// mask: bitmask matching ymir::peripheral::Button, 1=released / 0=pressed.
void setPadState(uint32 player, uint32 mask) {
    if (player < 2) {
        g_emu.padState[player] = static_cast<uint16>(mask);
    }
}

// --- Backup RAM persistence ------------------------------------------------

emscripten::val getBackupRAM() {
    if (!g_emu.saturn) {
        return emscripten::val::undefined();
    }
    const std::vector<uint8> data = g_emu.saturn->mem.GetInternalBackupRAM().ReadAll();
    return emscripten::val::global("Uint8Array").new_(
        emscripten::typed_memory_view(data.size(), const_cast<uint8 *>(data.data())));
}

bool setBackupRAM(emscripten::val data) {
    if (!g_emu.saturn) {
        return false;
    }
    const std::vector<uint8> buf = emscripten::convertJSArrayToNumberVector<uint8>(data);
    auto &bram = g_emu.saturn->mem.GetInternalBackupRAM();
    if (buf.size() != bram.Size()) {
        return false;
    }
    // BackupMemory::WriteByte takes a *bus* address (each data byte lives at even
    // addresses, it shifts right by 1 internally), so image offset i -> address i*2.
    for (uint32 i = 0; i < buf.size(); ++i) {
        bram.WriteByte(i * 2, buf[i]);
    }
    return true;
}

} // namespace

EMSCRIPTEN_BINDINGS(ymir_web) {
    using namespace emscripten;
    function("init", &init);
    function("getVersion", &getVersion);
    function("loadIPL", &loadIPL);
    function("loadDisc", &loadDisc);
    function("ejectDisc", &ejectDisc);
    function("reset", &reset);
    function("runFrame", &runFrame);
    function("getFramebuffer", &getFramebuffer);
    function("getFrameWidth", &getFrameWidth);
    function("getFrameHeight", &getFrameHeight);
    function("audioAvailable", &audioAvailable);
    function("readAudio", &readAudio);
    function("setPadState", &setPadState);
    function("getBackupRAM", &getBackupRAM);
    function("setBackupRAM", &setBackupRAM);
}
