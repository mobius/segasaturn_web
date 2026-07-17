#include "cdblock_window_base.hpp"

namespace app::ui {

CDBlockWindowBase::CDBlockWindowBase(SharedContext &context)
    : WindowBase(context)
    , m_cdblock(context.saturn.GetCDBlock())
    , m_tracer(context.tracers.CDBlock) {}

} // namespace app::ui
