#include "scsp_output_window.hpp"

namespace app::ui {

SCSPOutputWindow::SCSPOutputWindow(SharedContext &context)
    : SCSPWindowBase(context)
    , m_outputView(context) {

    m_windowConfig.name = "SCSP output";
    // m_windowConfig.flags = ImGuiWindowFlags_AlwaysAutoResize;
}

void SCSPOutputWindow::PrepareWindow() {
    ImGui::SetNextWindowSizeConstraints(ImVec2(200 * m_context.displayScale, 50 * m_context.displayScale),
                                        ImVec2(FLT_MAX, 200 * m_context.displayScale));
}

void SCSPOutputWindow::DrawContents() {
    m_outputView.Display(ImGui::GetContentRegionAvail());
}

} // namespace app::ui
