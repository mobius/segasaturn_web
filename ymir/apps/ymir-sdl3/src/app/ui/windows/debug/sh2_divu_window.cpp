#include "sh2_divu_window.hpp"

using namespace ymir;

namespace app::ui {

SH2DivisionUnitWindow::SH2DivisionUnitWindow(SharedContext &context, bool master)
    : SH2WindowBase(context, master)
    , m_divuRegsView(context, m_sh2)
    , m_divuStatsView(context, m_tracer)
    , m_divuTraceView(context, m_tracer) {

    m_windowConfig.name = fmt::format("{}SH2 division unit (DIVU)", master ? 'M' : 'S');
}

void SH2DivisionUnitWindow::PrepareWindow() {
    ImGui::SetNextWindowSizeConstraints(ImVec2(570 * m_context.displayScale, 356 * m_context.displayScale),
                                        ImVec2(570 * m_context.displayScale, FLT_MAX));
}

void SH2DivisionUnitWindow::DrawContents() {
    ImGui::SeparatorText("Registers");
    m_divuRegsView.Display();

    ImGui::SeparatorText("Statistics");
    m_divuStatsView.Display();

    ImGui::SeparatorText("Trace");
    m_divuTraceView.Display();
}

} // namespace app::ui
