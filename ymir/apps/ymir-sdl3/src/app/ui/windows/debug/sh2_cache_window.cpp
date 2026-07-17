#include "sh2_cache_window.hpp"

namespace app::ui {

SH2CacheWindow::SH2CacheWindow(SharedContext &context, bool master)
    : SH2WindowBase(context, master)
    , m_cacheRegView(context, m_sh2)
    , m_cacheEntriesView(context, m_sh2) {

    m_windowConfig.name = fmt::format("{}SH2 cache", master ? 'M' : 'S');
}

void SH2CacheWindow::PrepareWindow() {
    ImGui::SetNextWindowSizeConstraints(ImVec2(652 * m_context.displayScale, 246 * m_context.displayScale),
                                        ImVec2(652 * m_context.displayScale, FLT_MAX));
}

void SH2CacheWindow::DrawContents() {
    ImGui::SeparatorText("Cache Control Register");
    m_cacheRegView.Display();

    ImGui::SeparatorText("Entries");
    m_cacheEntriesView.Display();
}

} // namespace app::ui
