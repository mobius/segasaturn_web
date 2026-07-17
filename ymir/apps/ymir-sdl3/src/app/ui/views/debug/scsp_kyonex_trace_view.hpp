#pragma once

#include <app/shared_context.hpp>

namespace app::ui {

class SCSPKeyOnExecuteTraceView {
public:
    SCSPKeyOnExecuteTraceView(SharedContext &context);

    void Display();

private:
    SharedContext &m_context;
    SCSPTracer &m_tracer;
};

} // namespace app::ui
