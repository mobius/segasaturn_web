#pragma once

#include "sh2_window_base.hpp"

#include <app/ui/views/debug/sh2_dmac_channel_view.hpp>
#include <app/ui/views/debug/sh2_dmac_registers_view.hpp>

namespace app::ui {

class SH2DMAControllerWindow : public SH2WindowBase {
public:
    SH2DMAControllerWindow(SharedContext &context, bool master);

protected:
    void DrawContents() override;

private:
    SH2DMAControllerRegistersView m_dmacRegsView;
    SH2DMAControllerChannelView m_dmacChannel0View;
    SH2DMAControllerChannelView m_dmacChannel1View;
};

} // namespace app::ui
