#pragma once

#include "settings_view_base.hpp"

namespace app::ui {

class GUISettingsView : public SettingsViewBase {
public:
    GUISettingsView(SharedContext &context);

    void Display();
};

} // namespace app::ui
