#pragma once

#include "settings_view_base.hpp"

namespace app::ui {

class AudioSettingsView : public SettingsViewBase {
public:
    AudioSettingsView(SharedContext &context);

    void Display();
};

} // namespace app::ui
