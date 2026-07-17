#include "scsp_window_base.hpp"

namespace app::ui {

SCSPWindowBase::SCSPWindowBase(SharedContext &context)
    : WindowBase(context)
    , m_scsp(context.saturn.GetSCSP())
    , m_tracer(context.tracers.SCSP) {}

} // namespace app::ui
