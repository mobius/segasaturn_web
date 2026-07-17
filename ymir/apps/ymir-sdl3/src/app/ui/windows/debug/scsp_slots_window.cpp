#include "scsp_slots_window.hpp"

namespace app::ui {

SCSPSlotsWindow::SCSPSlotsWindow(SharedContext &context)
    : SCSPWindowBase(context)
    , m_slotsView(context) {

    m_windowConfig.name = "SCSP slots";
    m_windowConfig.flags = ImGuiWindowFlags_AlwaysAutoResize;
}

void SCSPSlotsWindow::DrawContents() {
    m_slotsView.Display();
}

} // namespace app::ui
