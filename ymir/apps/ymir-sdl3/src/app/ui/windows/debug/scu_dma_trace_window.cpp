#include "scu_dma_trace_window.hpp"

namespace app::ui {

SCUDMATraceWindow::SCUDMATraceWindow(SharedContext &context)
    : WindowBase(context)
    , m_dmaTraceView(context) {

    m_windowConfig.name = "SCU DMA trace";
}

void SCUDMATraceWindow::PrepareWindow() {
    ImGui::SetNextWindowSizeConstraints(ImVec2(425 * m_context.displayScale, 200 * m_context.displayScale),
                                        ImVec2(525 * m_context.displayScale, FLT_MAX));
}

void SCUDMATraceWindow::DrawContents() {
    m_dmaTraceView.Display();
}

} // namespace app::ui
