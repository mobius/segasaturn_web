#pragma once

#include "input_action.hpp"
#include "input_events.hpp"

namespace app::input {

// Number of simultaneous input elements allowed per bind
inline constexpr size_t kNumBindsPerInput = 5;

struct InputBind {
    InputBind(Action action)
        : action(action) {}

    const Action action;
    std::array<InputElement, kNumBindsPerInput> elements;
};

} // namespace app::input
