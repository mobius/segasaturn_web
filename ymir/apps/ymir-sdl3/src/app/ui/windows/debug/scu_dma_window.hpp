#pragma once

#include <app/ui/window_base.hpp>

#include <app/ui/views/debug/scu_dma_registers_view.hpp>
#include <app/ui/views/debug/scu_dma_state_view.hpp>

namespace app::ui {

class SCUDMAWindow : public WindowBase {
public:
    SCUDMAWindow(SharedContext &context);

protected:
    void DrawContents() override;

private:
    SCUDMARegistersView m_dmaRegsView;
    SCUDMAStateView m_dmaStateView;
};

} // namespace app::ui
