#pragma once

#include "sh2_window_base.hpp"

#include <app/ui/views/debug/sh2_cache_entries_view.hpp>
#include <app/ui/views/debug/sh2_cache_register_view.hpp>

namespace app::ui {

class SH2CacheWindow : public SH2WindowBase {
public:
    SH2CacheWindow(SharedContext &context, bool master);

protected:
    void PrepareWindow();
    void DrawContents();

private:
    SH2CacheRegisterView m_cacheRegView;
    SH2CacheEntriesView m_cacheEntriesView;
};

} // namespace app::ui
