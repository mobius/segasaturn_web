#pragma once

#include "scsp_kyonex_trace_window.hpp"
#include "scsp_output_window.hpp"
#include "scsp_slots_window.hpp"

namespace app::ui {

struct SCSPWindowSet {
    SCSPWindowSet(SharedContext &context)
        : output(context)
        , slots(context)
        , kyonexTrace(context) {}

    void DisplayAll() {
        output.Display();
        slots.Display();
        kyonexTrace.Display();
    }

    SCSPOutputWindow output;
    SCSPSlotsWindow slots;
    SCSPKeyOnExecuteTraceWindow kyonexTrace;
};

} // namespace app::ui
