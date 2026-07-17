#pragma once

#include <app/shared_context.hpp>

namespace app::ui {

class SCUDMAStateView {
public:
    SCUDMAStateView(SharedContext &context);

    void Display(uint8 channel);

private:
    SharedContext &m_context;
    ymir::scu::SCU &m_scu;
};

} // namespace app::ui
