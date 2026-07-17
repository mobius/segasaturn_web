#include "scu_dsp_window.hpp"

namespace app::ui {

SCUDSPWindow::SCUDSPWindow(SharedContext &context)
    : WindowBase(context)
    , m_regsView(context)
    , m_disasmView(context)
    , m_dataRAMView(context)
    , m_dmaRegsView(context)
    , m_dmaTraceView(context) {

    m_windowConfig.name = "SCU DSP";
}

void SCUDSPWindow::PrepareWindow() {
    ImGui::SetNextWindowSizeConstraints(ImVec2(1041 * m_context.displayScale, 368 * m_context.displayScale),
                                        ImVec2(FLT_MAX, FLT_MAX));
}

void SCUDSPWindow::DrawContents() {
    if (ImGui::BeginTable("scu_dsp", 2, ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_BordersInnerV)) {
        ImGui::TableSetupColumn("Registers/Disassembly", ImGuiTableColumnFlags_WidthStretch);
        ImGui::TableSetupColumn("DMA", ImGuiTableColumnFlags_WidthFixed, 310 * m_context.displayScale);

        ImGui::TableNextRow();
        if (ImGui::TableNextColumn()) {
            // ImGui::SeparatorText("Controls");
            // ImGui::TextUnformatted("(placeholder for controls)");

            ImGui::SeparatorText("Registers");
            m_regsView.Display();

            ImGui::SeparatorText("Disassembly");
            m_disasmView.Display();
        }
        if (ImGui::TableNextColumn()) {
            if (ImGui::BeginTabBar("right_pane")) {
                if (ImGui::BeginTabItem("Data RAM")) {
                    m_dataRAMView.Display();

                    ImGui::EndTabItem();
                }
                if (ImGui::BeginTabItem("DMA")) {
                    ImGui::SeparatorText("Registers");
                    m_dmaRegsView.Display();
                    ImGui::SeparatorText("Trace");
                    m_dmaTraceView.Display();

                    ImGui::EndTabItem();
                }

                ImGui::EndTabBar();
            }
        }

        ImGui::EndTable();
    }
}

} // namespace app::ui