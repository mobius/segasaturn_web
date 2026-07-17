#pragma once

#include "settings_view_base.hpp"

namespace app::ui {

class CartridgeSettingsView : public SettingsViewBase {
public:
    CartridgeSettingsView(SharedContext &context);

    void Display();

private:
    void DrawBackupRAMSettings();
    void DrawDRAMSettings();
    void DrawROMSettings();

    static void ProcessLoadBackupImage(void *userdata, std::filesystem::path file, int filter);
    static void ProcessLoadBackupImageError(void *userdata, const char *message, int filter);

    void LoadBackupImage(std::filesystem::path file);
    void ShowLoadBackupImageError(const char *message);

    static void ProcessLoadROMImage(void *userdata, std::filesystem::path file, int filter);
    static void ProcessLoadROMImageError(void *userdata, const char *message, int filter);

    void LoadROMImage(std::filesystem::path file);
    void ShowLoadROMImageError(const char *message);
};

} // namespace app::ui
