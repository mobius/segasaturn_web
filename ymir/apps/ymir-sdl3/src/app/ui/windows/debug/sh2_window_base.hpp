#pragma once

#include <app/ui/window_base.hpp>

#include <app/debug/sh2_tracer.hpp>

namespace app::ui {

class SH2WindowBase : public WindowBase {
public:
    SH2WindowBase(SharedContext &context, bool master);
    virtual ~SH2WindowBase() = default;

protected:
    ymir::sh2::SH2 &m_sh2;
    SH2Tracer &m_tracer;
};

} // namespace app::ui