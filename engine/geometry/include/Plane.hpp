#pragma once
#include "Vec3.hpp"

namespace yarep::geometry {
    struct Plane {
        math::Vec3 normal{};
        float distance = 0.0f;

        constexpr Plane() = default;

        constexpr Plane(const math::Vec3& normal, float distance) : normal(normal), distance(distance) {
        }

        constexpr bool operator==(const Plane& other) const {
            return normal == other.normal && distance == other.distance;
        }
    };
}
