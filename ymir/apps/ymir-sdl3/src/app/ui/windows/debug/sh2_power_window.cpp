#include "sh2_power_window.hpp"

using namespace ymir;

namespace app::ui {

SH2PowerWindow::SH2PowerWindow(SharedContext &context, bool master)
    : SH2WindowBase(context, master)
    , m_powerView(context, m_sh2) {

    m_windowConfig.name = fmt::format("{}SH2 power module", master ? 'M' : 'S');
    m_windowConfig.flags = ImGuiWindowFlags_AlwaysAutoResize;
}

void SH2PowerWindow::DrawContents() {
    m_powerView.Display();
}

} // namespace app::ui
