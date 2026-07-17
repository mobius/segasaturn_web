#pragma once

#include <app/ui/window_base.hpp>

#include <app/ui/views/debug/debug_output_view.hpp>

namespace app::ui {

class DebugOutputWindow : public WindowBase {
public:
    DebugOutputWindow(SharedContext &context);

protected:
    void PrepareWindow() override;
    void DrawContents() override;

private:
    DebugOutputView m_debugOutputView;
};

} // namespace app::ui
