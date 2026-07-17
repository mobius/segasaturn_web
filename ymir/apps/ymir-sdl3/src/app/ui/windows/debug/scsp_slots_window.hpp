#pragma once

#include "scsp_window_base.hpp"

#include <app/ui/views/debug/scsp_slots_view.hpp>

namespace app::ui {

class SCSPSlotsWindow : public SCSPWindowBase {
public:
    SCSPSlotsWindow(SharedContext &context);

protected:
    void DrawContents() override;

private:
    SCSPSlotsView m_slotsView;
};

} // namespace app::ui