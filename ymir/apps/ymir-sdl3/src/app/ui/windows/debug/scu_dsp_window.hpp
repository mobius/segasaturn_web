#pragma once

#include <app/ui/window_base.hpp>

#include <app/ui/views/debug/scu_dsp_data_ram_view.hpp>
#include <app/ui/views/debug/scu_dsp_disassembly_view.hpp>
#include <app/ui/views/debug/scu_dsp_dma_registers_view.hpp>
#include <app/ui/views/debug/scu_dsp_dma_trace_view.hpp>
#include <app/ui/views/debug/scu_dsp_registers_view.hpp>

namespace app::ui {

class SCUDSPWindow : public WindowBase {
public:
    SCUDSPWindow(SharedContext &context);

protected:
    void PrepareWindow() override;
    void DrawContents() override;

private:
    SCUDSPRegistersView m_regsView;
    SCUDSPDisassemblyView m_disasmView;
    SCUDSPDataRAMView m_dataRAMView;
    SCUDSPDMARegistersView m_dmaRegsView;
    SCUDSPDMATraceView m_dmaTraceView;
};

} // namespace app::ui
