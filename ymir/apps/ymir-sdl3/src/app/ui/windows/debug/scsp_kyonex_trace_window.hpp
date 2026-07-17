#pragma once

#include "scsp_window_base.hpp"

#include <app/ui/views/debug/scsp_kyonex_trace_view.hpp>

namespace app::ui {

class SCSPKeyOnExecuteTraceWindow : public SCSPWindowBase {
public:
    SCSPKeyOnExecuteTraceWindow(SharedContext &context);

protected:
    void PrepareWindow() override;
    void DrawContents() override;

private:
    SCSPKeyOnExecuteTraceView m_kyonexTraceView;
};

} // namespace app::ui