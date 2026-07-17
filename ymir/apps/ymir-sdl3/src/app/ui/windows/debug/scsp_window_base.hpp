#pragma once

#include <app/ui/window_base.hpp>

#include <app/debug/scsp_tracer.hpp>

namespace app::ui {

class SCSPWindowBase : public WindowBase {
public:
    SCSPWindowBase(SharedContext &context);
    virtual ~SCSPWindowBase() = default;

protected:
    ymir::scsp::SCSP &m_scsp;
    SCSPTracer &m_tracer;
};

} // namespace app::ui
