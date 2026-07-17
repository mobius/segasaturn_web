#include "debug_output_window.hpp"

#include <imgui.h>

namespace app::ui {

DebugOutputWindow::DebugOutputWindow(SharedContext &context)
    : WindowBase(context)
    , m_debugOutputView(context) {

    m_windowConfig.name = "Debug output";
}

void DebugOutputWindow::PrepareWindow() {
    ImGui::SetNextWindowSizeConstraints(ImVec2(365 * m_context.displayScale, 150 * m_context.displayScale),
                                        ImVec2(FLT_MAX, FLT_MAX));
}

void DebugOutputWindow::DrawContents() {
    m_debugOutputView.Display();
}

} // namespace app::ui
