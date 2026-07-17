// Node smoke test for the ymir-web WASM module (no browser required).
// Run: node web/test/smoke.cjs
// The emulator boots with a zeroed dummy IPL — it will not run real code,
// but all subsystems (CPU/VDP/SCSP/SMPC) tick, so we can verify the bridge API.

const path = require("path");
const createYmirModule = require(path.join(__dirname, "..", "..", "build-web", "ymir-web.js"));

let failures = 0;
function check(name, cond) {
    console.log(`${cond ? "PASS" : "FAIL"}  ${name}`);
    if (!cond) failures++;
}

(async () => {
    const M = await createYmirModule();

    check("init()", M.init() === true);
    const ver = M.getVersion();
    check(`getVersion() non-empty (got "${ver}")`, typeof ver === "string" && ver.length > 0);

    // IPL loading
    check("loadIPL rejects wrong size", M.loadIPL(new Uint8Array(1024)) === false);
    check("loadIPL accepts 512 KiB", M.loadIPL(new Uint8Array(512 * 1024)) === true);

    // Disc loading from MEMFS (invalid data must fail gracefully, not crash)
    M.FS.mkdir("/roms");
    M.FS.writeFile("/roms/bad.chd", new Uint8Array(4096));
    check("loadDisc rejects garbage", M.loadDisc("/roms/bad.chd") === false);

    // Power on and run frames
    M.reset(true);
    const FRAMES = 30;
    const t0 = Date.now();
    for (let i = 0; i < FRAMES; i++) M.runFrame();
    const elapsed = Date.now() - t0;
    console.log(`      ran ${FRAMES} frames in ${elapsed} ms (${(elapsed / FRAMES).toFixed(1)} ms/frame)`);

    const w = M.getFrameWidth(), h = M.getFrameHeight();
    check(`framebuffer produced (${w}x${h})`, w > 0 && h > 0);
    const fb = M.getFramebuffer();
    check("framebuffer view size == w*h*4", fb.length === w * h * 4);

    const avail = M.audioAvailable();
    check(`audio samples produced (${avail} stereo frames after ${FRAMES} emulated frames)`, avail > 0);
    const chunk = M.readAudio(1024);
    check("readAudio drains <= requested", chunk.length <= 2048 && chunk.length % 2 === 0);

    // Input (press A + Start on pad 1, then run more frames — must not crash)
    const BTN = { Start: 1 << 11, A: 1 << 10 };
    M.setPadState(0, 0xFFFF & ~BTN.Start & ~BTN.A);
    for (let i = 0; i < 5; i++) M.runFrame();
    M.setPadState(0, 0xFFFF);
    check("input injection survives frames", true);

    // Backup RAM roundtrip (32 KiB internal)
    const bram = M.getBackupRAM();
    check("getBackupRAM returns 32 KiB", bram.length === 32768);
    const modified = new Uint8Array(bram);
    modified[123] = 0x5A;
    check("setBackupRAM accepts 32 KiB image", M.setBackupRAM(modified) === true);
    check("backup RAM roundtrip preserves data", M.getBackupRAM()[123] === 0x5A);
    check("setBackupRAM rejects wrong size", M.setBackupRAM(new Uint8Array(16)) === false);

    console.log(failures === 0 ? "\nALL CHECKS PASSED" : `\n${failures} CHECK(S) FAILED`);
    process.exit(failures === 0 ? 0 : 1);
})().catch((e) => {
    console.error("FATAL", e);
    process.exit(1);
});
