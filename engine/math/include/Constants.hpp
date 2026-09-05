#pragma once
#include <numbers>

namespace yarep::math {
    constexpr float PI = std::numbers::pi_v<float>;
    constexpr float HALF_PI = std::numbers::pi_v<float> / 2;
    constexpr float EPSILON = 1e-5f;
}
