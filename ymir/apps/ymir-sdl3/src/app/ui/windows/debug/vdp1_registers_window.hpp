#pragma once

#include "vdp_window_base.hpp"

#include <app/ui/views/debug/vdp1_registers_view.hpp>

namespace app::ui {

class VDP1RegistersWindow : public VDPWindowBase {
public:
    VDP1RegistersWindow(SharedContext &context);

protected:
    void DrawContents() override;

private:
    VDP1RegistersView m_regsView;
};

} // namespace app::ui
