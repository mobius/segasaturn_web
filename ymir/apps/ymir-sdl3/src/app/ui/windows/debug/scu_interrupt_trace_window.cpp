#include "scu_interrupt_trace_window.hpp"

#include <imgui.h>

namespace app::ui {

SCUInterruptTraceWindow::SCUInterruptTraceWindow(SharedContext &context)
    : WindowBase(context)
    , m_intrTraceView(context) {

    m_windowConfig.name = "SCU interrupt trace";
}

void SCUInterruptTraceWindow::PrepareWindow() {
    ImGui::SetNextWindowSizeConstraints(ImVec2(300 * m_context.displayScale, 200 * m_context.displayScale),
                                        ImVec2(450 * m_context.displayScale, FLT_MAX));
}

void SCUInterruptTraceWindow::DrawContents() {
    ImGui::SeparatorText("Interrupt trace");
    m_intrTraceView.Display();
}

} // namespace app::ui
