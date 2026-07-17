#pragma once

#include <app/shared_context.hpp>

#include <app/ui/model/debug/sh2_debugger_model.hpp>

namespace app::ui {

class SH2RegistersView {
public:
    SH2RegistersView(SharedContext &context, ymir::sh2::SH2 &sh2, SH2DebuggerModel &model);

    void Display();

    float GetViewWidth();

private:
    SharedContext &m_context;
    ymir::sh2::SH2 &m_sh2;
    SH2DebuggerModel &m_model;
};

} // namespace app::ui
