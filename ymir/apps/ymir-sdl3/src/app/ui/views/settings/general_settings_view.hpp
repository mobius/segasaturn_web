#pragma once

#include "settings_view_base.hpp"

namespace app::ui {

class GeneralSettingsView : public SettingsViewBase {
public:
    GeneralSettingsView(SharedContext &context);

    void Display();

private:
    ProfilePath m_selectedProfPath;

    static void ProcessPathOverrideSelection(void *userdata, std::filesystem::path file, int filter);
    static void ProcessPathOverrideSelectionError(void *userdata, const char *message, int filter);

    void SelectPathOverride(std::filesystem::path file);
    void ShowPathOverrideSelectionError(const char *message);
};

} // namespace app::ui
