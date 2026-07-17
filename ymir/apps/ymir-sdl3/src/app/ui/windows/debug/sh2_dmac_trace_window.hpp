#pragma once

#include "sh2_window_base.hpp"

#include <app/ui/views/debug/sh2_dmac_trace_view.hpp>

namespace app::ui {

class SH2DMAControllerTraceWindow : public SH2WindowBase {
public:
    SH2DMAControllerTraceWindow(SharedContext &context, bool master);

protected:
    void PrepareWindow() override;
    void DrawContents() override;

private:
    SH2DMAControllerChannelTraceView m_dmac0TraceView;
    SH2DMAControllerChannelTraceView m_dmac1TraceView;
};

} // namespace app::ui
