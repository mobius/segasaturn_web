#pragma once

#include <app/shared_context.hpp>

namespace app::ui {

class SCUDSPDataRAMView {
public:
    SCUDSPDataRAMView(SharedContext &context);

    void Display();

private:
    SharedContext &m_context;
    ymir::scu::SCU &m_scu;
};

} // namespace app::ui
