#pragma once

#include "sh2_window_base.hpp"

#include <app/ui/views/debug/sh2_frt_view.hpp>
#include <app/ui/views/debug/sh2_wdt_view.hpp>

namespace app::ui {

class SH2TimersWindow : public SH2WindowBase {
public:
    SH2TimersWindow(SharedContext &context, bool master);

protected:
    void DrawContents() override;

private:
    SH2FreeRunningTimerView m_frtView;
    SH2WatchdogTimerView m_wdtView;
};

} // namespace app::ui