#pragma once

#include <app/shared_context.hpp>

#include <app/debug/scu_tracer.hpp>

namespace app::ui {

class SCUInterruptTraceView {
public:
    SCUInterruptTraceView(SharedContext &context);

    void Display();

private:
    SharedContext &m_context;
    SCUTracer &m_tracer;
};

} // namespace app::ui
