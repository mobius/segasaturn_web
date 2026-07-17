#pragma once

#include "sh2_window_base.hpp"

#include <app/ui/views/debug/sh2_interrupts_view.hpp>

namespace app::ui {

class SH2InterruptsWindow : public SH2WindowBase {
public:
    SH2InterruptsWindow(SharedContext &context, bool master);

protected:
    void DrawContents() override;

private:
    SH2InterruptsView m_intrView;
};

} // namespace app::ui
