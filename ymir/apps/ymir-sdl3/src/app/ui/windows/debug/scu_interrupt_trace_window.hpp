#pragma once

#include <app/ui/window_base.hpp>

#include <app/ui/views/debug/scu_interrupt_trace_view.hpp>

namespace app::ui {

class SCUInterruptTraceWindow : public WindowBase {
public:
    SCUInterruptTraceWindow(SharedContext &context);

protected:
    void PrepareWindow() override;
    void DrawContents() override;

private:
    SCUInterruptTraceView m_intrTraceView;
};

} // namespace app::ui
