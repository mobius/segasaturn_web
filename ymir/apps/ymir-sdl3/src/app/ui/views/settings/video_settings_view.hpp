#pragma once

#include "settings_view_base.hpp"

namespace app::ui {

class VideoSettingsView : public SettingsViewBase {
public:
    VideoSettingsView(SharedContext &context);

    void Display();
};

} // namespace app::ui
