#pragma once

#include <app/shared_context.hpp>

namespace app::ui {

class SCUInterruptsView {
public:
    SCUInterruptsView(SharedContext &context);

    void Display();

private:
    SharedContext &m_context;
    ymir::scu::SCU &m_scu;

    void DisplayInternalInterrupts();
    void DisplayExternalInterrupts();
};

} // namespace app::ui
