#include "sh2_interrupt_trace_window.hpp"

using namespace ymir;

namespace app::ui {

SH2InterruptTraceWindow::SH2InterruptTraceWindow(SharedContext &context, bool master)
    : SH2WindowBase(context, master)
    , m_intrTraceView(context, m_tracer) {

    m_windowConfig.name = fmt::format("{}SH2 interrupt trace", master ? 'M' : 'S');
}

void SH2InterruptTraceWindow::PrepareWindow() {
    ImGui::SetNextWindowSizeConstraints(ImVec2(250 * m_context.displayScale, 200 * m_context.displayScale),
                                        ImVec2(600 * m_context.displayScale, FLT_MAX));
}

void SH2InterruptTraceWindow::DrawContents() {
    m_intrTraceView.Display();
}

} // namespace app::ui
