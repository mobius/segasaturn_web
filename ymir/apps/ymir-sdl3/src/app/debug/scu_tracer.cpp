#include "scu_tracer.hpp"

namespace app {

void SCUTracer::ClearDebugMessages() {
    debugMessages.Clear();
    m_debugMessageBuffer.clear();
}

void SCUTracer::ClearInterrupts() {
    interrupts.Clear();
    m_interruptCounter = 0;
}

void SCUTracer::ClearDMATransfers() {
    dmaTransfers.Clear();
    m_dmaCounter = 0;
}

void SCUTracer::ClearDSPDMATransfers() {
    dspDmaTransfers.Clear();
    m_dspDmaCounter = 0;
}

void SCUTracer::RaiseInterrupt(uint8 index, uint8 level) {
    if (!traceInterrupts) {
        return;
    }

    interrupts.Write({m_interruptCounter++, index, level, false});
}

void SCUTracer::AcknowledgeInterrupt(uint8 index) {
    if (!traceInterrupts) {
        return;
    }

    auto &intr = interrupts.GetLast();
    if (intr.index == index) {
        intr.acknowledged = true;
    }
}

void SCUTracer::DebugPortWrite(uint8 ch) {
    if (ch == '\n') {
        debugMessages.Write(m_debugMessageBuffer);
        m_debugMessageBuffer.clear();
    } else if (ch != '\r') {
        m_debugMessageBuffer.push_back(ch);
    }
}

void SCUTracer::DMA(uint8 channel, uint32 srcAddr, uint32 dstAddr, uint32 xferCount, uint32 srcAddrInc,
                    uint32 dstAddrInc, bool indirect, uint32 indirectAddr) {
    if (!traceDMA) {
        return;
    }

    dmaTransfers.Write(
        {m_dmaCounter++, srcAddr, dstAddr, xferCount, srcAddrInc, dstAddrInc, indirectAddr, indirect, channel});
}

void SCUTracer::DSPDMA(bool toD0, uint32 addrD0, uint8 addrDSP, uint8 count, uint8 addrInc, bool hold) {
    if (!traceDSPDMA) {
        return;
    }

    dspDmaTransfers.Write({m_dspDmaCounter++, addrD0, addrDSP, count, addrInc, toD0, hold});
}

} // namespace app
