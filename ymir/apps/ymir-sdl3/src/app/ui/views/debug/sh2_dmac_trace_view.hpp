#pragma once

#include <app/shared_context.hpp>

#include <app/debug/sh2_tracer.hpp>

namespace app::ui {

class SH2DMAControllerChannelTraceView {
public:
    SH2DMAControllerChannelTraceView(SharedContext &context, int index, SH2Tracer &tracer);

    void Display();

private:
    SharedContext &m_context;
    const int m_index;
    SH2Tracer &m_tracer;

    void DisplayStatistics();
    void DisplayTrace();
};

} // namespace app::ui
