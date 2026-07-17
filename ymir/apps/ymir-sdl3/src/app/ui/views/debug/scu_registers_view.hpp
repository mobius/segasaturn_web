#pragma once

#include <app/shared_context.hpp>

namespace app::ui {

class SCURegistersView {
public:
    SCURegistersView(SharedContext &context);

    void Display();

private:
    ymir::scu::SCU &m_scu;
};

} // namespace app::ui
