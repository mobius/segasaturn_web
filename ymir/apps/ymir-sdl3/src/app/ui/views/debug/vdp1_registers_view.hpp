#pragma once

#include <app/shared_context.hpp>

namespace app::ui {

class VDP1RegistersView {
public:
    VDP1RegistersView(SharedContext &context, ymir::vdp::VDP &vdp);

    void Display();

private:
    SharedContext &m_context;
    ymir::vdp::VDP &m_vdp;
};

} // namespace app::ui
