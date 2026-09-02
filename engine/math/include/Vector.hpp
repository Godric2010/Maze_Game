//
// Created by sebastian on 02.09.26.
//

#pragma once
#include <cmath>
#include "Vec2.hpp"

namespace yarep::math {
    constexpr float length_squared(const Vec2& vec) {
        return vec.x * vec.x + vec.y * vec.y;
    }

    inline float length(const Vec2& vec) {
        return std::sqrt(length_squared(vec));
    }
}
