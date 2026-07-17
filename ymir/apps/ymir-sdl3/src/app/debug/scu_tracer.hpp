#pragma once

#include <ymir/debug/scu_tracer_base.hpp>

#include <util/ring_buffer.hpp>

#include <string>
#include <string_view>

namespace app {

struct SCUTracer final : ymir::debug::ISCUTracer {
    std::string_view GetDebugMessageBuffer() const {
        return m_debugMessageBuffer;
    }

    void ClearDebugMessages();
    void ClearInterrupts();
    void ClearDMATransfers();
    void ClearDSPDMATransfers();

    void DebugPortWrite(uint8 ch);

    bool traceInterrupts = false;
    bool traceDMA = false;
    bool traceDSPDMA = false;

    struct InterruptInfo {
        uint32 counter;
        uint8 index;
        uint8 level;
        bool acknowledged;
    };

    struct DMAInfo {
        uint32 counter;
        uint32 srcAddr;
        uint32 dstAddr;
        uint32 xferCount;
        uint32 srcAddrInc;
        uint32 dstAddrInc;
        uint32 indirectAddr;
        bool indirect;
        uint8 channel;
    };

    struct DSPDMAInfo {
        uint32 counter;
        uint32 addrD0;
        uint8 addrDSP;
        uint8 count;
        uint8 addrInc;
        bool toD0;
        bool hold;
    };

    util::RingBuffer<InterruptInfo, 1024> interrupts;
    util::RingBuffer<DMAInfo, 1024> dmaTransfers;
    util::RingBuffer<DSPDMAInfo, 1024> dspDmaTransfers;
    util::RingBuffer<std::string, 1024> debugMessages;

private:
    uint32 m_interruptCounter = 0;
    uint32 m_dmaCounter = 0;
    uint32 m_dspDmaCounter = 0;
    std::string m_debugMessageBuffer;

    // -------------------------------------------------------------------------
    // ISCUTracer implementation

    void RaiseInterrupt(uint8 index, uint8 level) final;
    void AcknowledgeInterrupt(uint8 index) final;

    void DMA(uint8 channel, uint32 srcAddr, uint32 dstAddr, uint32 xferCount, uint32 srcAddrInc, uint32 dstAddrInc,
             bool indirect, uint32 indirectAddr) final;

    void DSPDMA(bool toD0, uint32 addrD0, uint8 addrDSP, uint8 count, uint8 addrInc, bool hold) final;
};

} // namespace app
