#pragma once

#include "cdblock_window_base.hpp"

#include <app/ui/views/debug/cdblock_filters_view.hpp>

namespace app::ui {

class CDBlockFiltersWindow : public CDBlockWindowBase {
public:
    CDBlockFiltersWindow(SharedContext &context);

protected:
    void PrepareWindow() override;
    void DrawContents() override;

private:
    CDBlockFiltersView m_filtersView;
};

} // namespace app::ui
