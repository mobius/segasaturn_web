#pragma once

#include "settings_view_base.hpp"

#include <filesystem>

namespace app::ui {

class IPLSettingsView : public SettingsViewBase {
public:
    IPLSettingsView(SharedContext &context);

    void Display();

private:
    static void ProcessLoadIPLROM(void *userdata, std::filesystem::path file, int filter);
    static void ProcessLoadIPLROMError(void *userdata, const char *message, int filter);

    void LoadIPLROM(std::filesystem::path file);
    void ShowIPLROMLoadError(const char *message);
};

} // namespace app::ui
