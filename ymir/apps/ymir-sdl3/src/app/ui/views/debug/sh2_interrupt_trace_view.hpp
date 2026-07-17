#pragma once

#include <app/shared_context.hpp>

#include <app/debug/sh2_tracer.hpp>

namespace app::ui {

class SH2InterruptTraceView {
public:
    SH2InterruptTraceView(SharedContext &context, SH2Tracer &tracer);

    void Display();

private:
    SharedContext &m_context;
    SH2Tracer &m_tracer;
};

} // namespace app::ui
