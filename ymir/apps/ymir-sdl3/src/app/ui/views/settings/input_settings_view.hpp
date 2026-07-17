#pragma once

#include "settings_view_base.hpp"

namespace app::ui {

class InputSettingsView : public SettingsViewBase {
public:
    InputSettingsView(SharedContext &context);

    void Display();
};

} // namespace app::ui
