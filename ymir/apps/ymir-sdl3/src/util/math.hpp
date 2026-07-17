#pragma once

#include <ymir/util/inline.hpp>

#include <cmath>

namespace util {

FORCE_INLINE double RoundToMultiple(double value, double multiple) {
    return std::round(value / multiple) * multiple;
}

} // namespace util
