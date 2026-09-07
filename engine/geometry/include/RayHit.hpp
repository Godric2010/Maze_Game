#pragma once
#include "Vec3.hpp"

namespace yarep::geometry {
    struct RayHit {
        float distance = 0.0f;
        math::Vec3 point{};
        math::Vec3 normal{};

        constexpr RayHit() = default;

        constexpr RayHit(float distance, math::Vec3 point, math::Vec3 normal) : distance(distance),
            point(point),
            normal(normal) {
        }

        constexpr bool operator==(const RayHit& other) const {
            return distance == other.distance && point == other.point && normal == other.normal;
        }
    };
}
