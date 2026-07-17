#pragma once

#include "settings_view_base.hpp"

namespace app::ui {

class SystemSettingsView : public SettingsViewBase {
public:
    SystemSettingsView(SharedContext &context);

    void Display();

private:
    static void ProcessLoadBackupImage(void *userdata, std::filesystem::path file, int filter);
    static void ProcessLoadBackupImageError(void *userdata, const char *message, int filter);

    void LoadBackupImage(std::filesystem::path file);
    void ShowLoadBackupImageError(const char *message);

    bool m_bupSettingsDirty = false;
};

} // namespace app::ui
