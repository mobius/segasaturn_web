#pragma once

#include "scsp_window_base.hpp"

#include <app/ui/views/debug/scsp_output_view.hpp>

namespace app::ui {

class SCSPOutputWindow : public SCSPWindowBase {
public:
    SCSPOutputWindow(SharedContext &context);

protected:
    void PrepareWindow() override;
    void DrawContents() override;

private:
    SCSPOutputView m_outputView;
};

} // namespace app::ui