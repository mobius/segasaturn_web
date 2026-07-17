#include "vdp1_registers_window.hpp"

namespace app::ui {

VDP1RegistersWindow::VDP1RegistersWindow(SharedContext &context)
    : VDPWindowBase(context)
    , m_regsView(context, m_vdp) {

    m_windowConfig.name = "VDP1 registers";
    m_windowConfig.flags = ImGuiWindowFlags_AlwaysAutoResize;
}

void VDP1RegistersWindow::DrawContents() {
    m_regsView.Display();
}

} // namespace app::ui
