#include "vdp2_layer_visibility_window.hpp"

namespace app::ui {

VDP2LayerVisibilityWindow::VDP2LayerVisibilityWindow(SharedContext &context)
    : VDPWindowBase(context)
    , m_layerVisibilityView(context, m_vdp) {

    m_windowConfig.name = "VDP2 layer visibility";
    m_windowConfig.flags = ImGuiWindowFlags_AlwaysAutoResize;
}

void VDP2LayerVisibilityWindow::DrawContents() {
    m_layerVisibilityView.Display();
}

} // namespace app::ui
