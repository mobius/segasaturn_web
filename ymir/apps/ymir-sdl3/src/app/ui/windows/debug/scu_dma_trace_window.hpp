#pragma once

#include <app/ui/window_base.hpp>

#include <app/ui/views/debug/scu_dma_trace_view.hpp>

#include <array>

namespace app::ui {

class SCUDMATraceWindow : public WindowBase {
public:
    SCUDMATraceWindow(SharedContext &context);

protected:
    void PrepareWindow() override;
    void DrawContents() override;

private:
    SCUDMATraceView m_dmaTraceView;
};

} // namespace app::ui
