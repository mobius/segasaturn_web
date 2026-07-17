#include "scsp_tracer.hpp"

namespace app {

void SCSPTracer::SlotSample(uint32 index, sint16 output) {
    slotOutputs[index].Write(output);
    if (index == 31) {
        ++m_sampleCounter;
    }
}

void SCSPTracer::KeyOnExecute(uint32 slotsMask) {
    kyonexTrace.Write({.sampleCounter = m_sampleCounter, .slotsMask = slotsMask});
}

} // namespace app
