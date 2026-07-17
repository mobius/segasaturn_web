#pragma once

#include <app/shared_context.hpp>

#include <ymir/hw/sh2/sh2_dmac.hpp>

namespace app::ui {

class SH2DMAControllerChannelView {
public:
    SH2DMAControllerChannelView(SharedContext &context, ymir::sh2::DMAChannel &channel, int index);

    void Display();

private:
    SharedContext &m_context;
    ymir::sh2::DMAChannel &m_channel;
    const int m_index;
};

} // namespace app::ui
