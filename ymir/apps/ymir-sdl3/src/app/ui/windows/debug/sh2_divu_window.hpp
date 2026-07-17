#pragma once

#include "sh2_window_base.hpp"

#include <app/ui/views/debug/sh2_divu_registers_view.hpp>
#include <app/ui/views/debug/sh2_divu_statistics_view.hpp>
#include <app/ui/views/debug/sh2_divu_trace_view.hpp>

namespace app::ui {

class SH2DivisionUnitWindow : public SH2WindowBase {
public:
    SH2DivisionUnitWindow(SharedContext &context, bool master);

protected:
    void PrepareWindow() override;
    void DrawContents() override;

private:
    SH2DivisionUnitRegistersView m_divuRegsView;
    SH2DivisionUnitStatisticsView m_divuStatsView;
    SH2DivisionUnitTraceView m_divuTraceView;
};

} // namespace app::ui
