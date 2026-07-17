#pragma once

#include <app/shared_context.hpp>

namespace app::ui {

class SCUDSPRegistersView {
public:
    SCUDSPRegistersView(SharedContext &context);

    void Display();

private:
    SharedContext &m_context;
    ymir::scu::SCU &m_scu;
};

} // namespace app::ui
