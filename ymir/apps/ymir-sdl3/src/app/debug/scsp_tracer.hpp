#pragma once

#include <ymir/debug/scsp_tracer_base.hpp>

#include <util/ring_buffer.hpp>

#include <array>

namespace app {

struct SCSPTracer final : ymir::debug::ISCSPTracer {
    struct KeyOnExecuteInfo {
        uint64 sampleCounter;
        uint32 slotsMask;
    };

    std::array<util::RingBuffer<sint16, 2048>, 32> slotOutputs;
    util::RingBuffer<KeyOnExecuteInfo, 2048> kyonexTrace;

    uint64 GetSampleCounter() const noexcept {
        return m_sampleCounter;
    }

    void ClearAll() {
        for (auto &slot : slotOutputs) {
            slot.Clear();
        }
        m_sampleCounter = 0;
    }

private:
    uint64 m_sampleCounter = 0;

    // -------------------------------------------------------------------------
    // ISCSPTracer implementation

    void SlotSample(uint32 index, sint16 output) final;
    void KeyOnExecute(uint32 slotsMask) final;
};

} // namespace app
