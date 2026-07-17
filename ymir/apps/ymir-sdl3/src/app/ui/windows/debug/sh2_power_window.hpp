#pragma once

#include "sh2_window_base.hpp"

#include <app/ui/views/debug/sh2_power_view.hpp>

namespace app::ui {

class SH2PowerWindow : public SH2WindowBase {
public:
    SH2PowerWindow(SharedContext &context, bool master);

protected:
    void DrawContents() override;

private:
    SH2PowerView m_powerView;
};

} // namespace app::ui