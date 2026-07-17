#pragma once

#include "sh2_window_base.hpp"

#include <app/ui/views/debug/sh2_interrupt_trace_view.hpp>

namespace app::ui {

class SH2InterruptTraceWindow : public SH2WindowBase {
public:
    SH2InterruptTraceWindow(SharedContext &context, bool master);

protected:
    void PrepareWindow() override;
    void DrawContents() override;

private:
    SH2InterruptTraceView m_intrTraceView;
};

} // namespace app::ui
