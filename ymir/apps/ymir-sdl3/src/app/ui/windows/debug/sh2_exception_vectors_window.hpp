#pragma once

#include "sh2_window_base.hpp"

#include <app/ui/views/debug/sh2_exception_vectors_view.hpp>

namespace app::ui {

class SH2ExceptionVectorsWindow : public SH2WindowBase {
public:
    SH2ExceptionVectorsWindow(SharedContext &context, bool master);

protected:
    void PrepareWindow();
    void DrawContents();

private:
    SH2ExceptionVectorsView m_excptVecView;
};

} // namespace app::ui
