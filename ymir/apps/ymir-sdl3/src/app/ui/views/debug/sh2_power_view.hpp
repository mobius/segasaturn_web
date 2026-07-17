#pragma once

#include <app/shared_context.hpp>

namespace app::ui {

class SH2PowerView {
public:
    SH2PowerView(SharedContext &context, ymir::sh2::SH2 &sh2);

    void Display();

private:
    SharedContext &m_context;
    ymir::sh2::SH2 &m_sh2;
};

} // namespace app::ui
