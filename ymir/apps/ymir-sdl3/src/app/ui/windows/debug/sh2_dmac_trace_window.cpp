#include "sh2_dmac_trace_window.hpp"

namespace app::ui {

SH2DMAControllerTraceWindow::SH2DMAControllerTraceWindow(SharedContext &context, bool master)
    : SH2WindowBase(context, master)
    , m_dmac0TraceView(context, 0, m_tracer)
    , m_dmac1TraceView(context, 1, m_tracer) {

    m_windowConfig.name = fmt::format("{}SH2 DMA controller trace", master ? 'M' : 'S');
}

void SH2DMAControllerTraceWindow::PrepareWindow() {
    ImGui::SetNextWindowSizeConstraints(ImVec2(900 * m_context.displayScale, 279 * m_context.displayScale),
                                        ImVec2(1121 * m_context.displayScale, FLT_MAX));
}

void SH2DMAControllerTraceWindow::DrawContents() {
    ImGui::Checkbox("Enable", &m_tracer.traceDMA);
    ImGui::SameLine();
    ImGui::TextDisabled("(?)");
    if (ImGui::BeginItemTooltip()) {
        ImGui::TextUnformatted("You must also enable tracing in Debug > Enable tracing (F11)");
        ImGui::EndTooltip();
    }
    ImGui::SameLine();
    if (ImGui::Button("Clear all##trace")) {
        m_tracer.dmaTransfers[0].Clear();
        m_tracer.dmaTransfers[1].Clear();
        m_tracer.dmaStats[0].Clear();
        m_tracer.dmaStats[1].Clear();
        m_tracer.ResetDMACounter(0);
        m_tracer.ResetDMACounter(1);
    }

    if (ImGui::BeginTable("dmac_traces", 2,
                          ImGuiTableFlags_SizingStretchSame | ImGuiTableFlags_ScrollY |
                              ImGuiTableFlags_BordersInnerV)) {
        ImGui::TableNextRow();

        if (ImGui::TableNextColumn()) {
            m_dmac0TraceView.Display();
        }
        if (ImGui::TableNextColumn()) {
            m_dmac1TraceView.Display();
        }

        ImGui::EndTable();
    }
}

} // namespace app::ui
