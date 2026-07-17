#include "sh2_window_base.hpp"

namespace app::ui {

SH2WindowBase::SH2WindowBase(SharedContext &context, bool master)
    : WindowBase(context)
    , m_sh2(context.saturn.GetSH2(master))
    , m_tracer(master ? context.tracers.masterSH2 : context.tracers.slaveSH2) {}

} // namespace app::ui
