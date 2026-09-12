#pragma once
#include <cmath>
#include "Angle.hpp"
#include "Constants.hpp"

namespace yarep::math {
    inline Angle normalize_angle(Angle angle) {
        constexpr float full_circle_rad = 2.0f * PI;
        float radians = std::fmod(angle.radians(), full_circle_rad);
        if (radians < 0.0f) {
            radians += full_circle_rad;
        }
        constexpr float epsilon = 1e-5f;
        if (std::abs(radians) <= epsilon || std::abs(radians - full_circle_rad) <= epsilon) {
            radians = 0.0f;
        }
        return Angle::from_radians(radians);
    }

    inline Angle delta_angle(Angle lhs, Angle rhs) {
        constexpr auto half_circle_rad = PI;
        const auto lhs_normalized = normalize_angle(lhs);
        const auto rhs_normalized = normalize_angle(rhs);

        auto delta = (rhs_normalized - lhs_normalized).radians();
        if (delta > half_circle_rad) {
            delta -= half_circle_rad * 2;
        } else if (delta < -half_circle_rad) {
            delta += half_circle_rad * 2;
        }


        return Angle::from_radians(delta);
    }

    inline Angle lerp(Angle a, Angle b, const float t) {
        const auto delta = delta_angle(a, b);
        const auto midway_rad = a.radians() + t * delta.radians();
        return Angle::from_radians(midway_rad);
    }

    inline float sin(Angle angle) {
        return std::sin(angle.radians());
    }

    inline float cos(Angle angle) {
        return std::cos(angle.radians());
    }

    inline float tan(Angle angle) {
        return std::tan(angle.radians());
    }

    inline bool nearly_equal(Angle a, Angle b, float tolerance) {
        const auto delta = std::abs(delta_angle(a, b).radians());
        return delta < tolerance;
    }
}
