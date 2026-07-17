#pragma once

#include <app/shared_context.hpp>

namespace app::ui::widgets {

struct RewindBarStyle {
    struct Colors {
        ImVec4 background;
        ImVec4 border;
        ImVec4 bar;
        ImVec4 secondsMarker;
        ImVec4 text;
    } colors;

    float padding = 10.0f;
    float height = 50.0f;
    float rounding = 2.0f;
    float borderThickness = 2.0f;
    float secondsMarkerThickness = 1.5f;
};

#define C(r, g, b, a) (r / 255.0f), (g / 255.0f), (b / 255.0f), a

inline RewindBarStyle g_defaultRewindBarStyle = {
    .colors =
        {
            .background{C(21, 31, 33, 0.67f)},
            .border{C(87, 149, 255, 0.85f)},
            .bar{C(34, 115, 255, 0.75f)},
            .secondsMarker{C(15, 63, 145, 0.75f)},
            .text{C(191, 215, 255, 1.00f)},
        },
};

#undef C

void RewindBar(SharedContext &context, float alpha = 1.0f, const RewindBarStyle &style = g_defaultRewindBarStyle);

} // namespace app::ui::widgets
