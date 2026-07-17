#pragma once

#include <app/shared_context.hpp>

namespace app::ui {

class SH2InterruptsView {
public:
    SH2InterruptsView(SharedContext &context, ymir::sh2::SH2 &sh2);

    void Display();

private:
    SharedContext &m_context;
    ymir::sh2::SH2 &m_sh2;

    uint8 m_extIntrVector = 0x0;
    uint8 m_extIntrLevel = 0x0;
};

} // namespace app::ui
