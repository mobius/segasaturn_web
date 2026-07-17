#include "scsp_kyonex_trace_window.hpp"

namespace app::ui {

SCSPKeyOnExecuteTraceWindow::SCSPKeyOnExecuteTraceWindow(SharedContext &context)
    : SCSPWindowBase(context)
    , m_kyonexTraceView(context) {

    m_windowConfig.name = "SCSP KYONEX trace";
    // m_windowConfig.flags = ImGuiWindowFlags_AlwaysAutoResize;
}

void SCSPKeyOnExecuteTraceWindow::PrepareWindow() {
    ImGui::SetNextWindowSizeConstraints(ImVec2(300 * m_context.displayScale, 100 * m_context.displayScale),
                                        ImVec2(450 * m_context.displayScale, FLT_MAX));
}

void SCSPKeyOnExecuteTraceWindow::DrawContents() {
    m_kyonexTraceView.Display();
}

} // namespace app::ui
