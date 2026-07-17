#include "scsp_kyonex_trace_view.hpp"

#include <cinttypes>

using namespace ymir;

namespace app::ui {

SCSPKeyOnExecuteTraceView::SCSPKeyOnExecuteTraceView(SharedContext &context)
    : m_context(context)
    , m_tracer(context.tracers.SCSP) {}

void SCSPKeyOnExecuteTraceView::Display() {
    ImGui::PushFont(m_context.fonts.monospace.regular, m_context.fontSizes.medium);
    const float hexCharWidth = ImGui::CalcTextSize("F").x;
    ImGui::PopFont();

    ImGui::BeginGroup();

    if (ImGui::BeginTable("kyonex_trace", 2, ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_ScrollY)) {
        ImGui::TableSetupColumn("#", ImGuiTableColumnFlags_WidthStretch);
        ImGui::TableSetupColumn("KYONB 0 to 31", ImGuiTableColumnFlags_WidthFixed, hexCharWidth * 32);
        ImGui::TableSetupScrollFreeze(1, 1);
        ImGui::TableHeadersRow();

        const uint32 count = m_tracer.kyonexTrace.Count();
        for (uint32 i = 0; i < count; ++i) {
            const auto &trace = m_tracer.kyonexTrace.ReadReverse(i);
            ImGui::TableNextRow();
            if (ImGui::TableNextColumn()) {
                ImGui::PushFont(m_context.fonts.monospace.regular, m_context.fontSizes.medium);
                ImGui::Text("%" PRIu64, trace.sampleCounter);
                ImGui::PopFont();
            }
            if (ImGui::TableNextColumn()) {
                std::array<char, 32> display{};
                for (uint32 j = 0; j < 32; ++j) {
                    if (trace.slotsMask & (1u << j)) {
                        display[j] = '+';
                    } else {
                        display[j] = '-';
                    }
                }
                ImGui::PushFont(m_context.fonts.monospace.regular, m_context.fontSizes.medium);
                ImGui::TextUnformatted(std::string(display.begin(), display.end()).c_str());
                ImGui::PopFont();
            }
        }

        ImGui::EndTable();
    }

    ImGui::EndGroup();
}

} // namespace app::ui
