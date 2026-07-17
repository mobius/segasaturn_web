#pragma once

#include <app/shared_context.hpp>

namespace app::ui {

class SH2ExceptionVectorsView {
public:
    SH2ExceptionVectorsView(SharedContext &context, ymir::sh2::SH2 &sh2);

    void Display();

    float GetWidth() const;

private:
    SharedContext &m_context;
    ymir::sh2::SH2 &m_sh2;

    bool m_useVBR = true;
    uint32 m_customAddress = 0x00000000;

    // Split into 2**m_columnShift columns
    uint32 m_columnShift = 2;
};

} // namespace app::ui
