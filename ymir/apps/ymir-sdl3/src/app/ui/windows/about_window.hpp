#pragma once

#include <app/ui/window_base.hpp>

namespace app::ui {

class AboutWindow : public WindowBase {
public:
    AboutWindow(SharedContext &context);

protected:
    void PrepareWindow() override;
    void DrawContents() override;

private:
    void DrawAboutTab();
    void DrawDependenciesTab();
    void DrawAcknowledgementsTab();
};

} // namespace app::ui
