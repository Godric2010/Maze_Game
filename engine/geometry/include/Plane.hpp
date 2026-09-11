#pragma once

namespace yarep::geometry {
    struct Plane {
        math::Vec3 normal{};
        float distance = 0.0f;

        constexpr Plane() = default;

        constexpr Plane(const math::Vec3& normal, float distance) : normal(normal), distance(distance) {
        }
    };
}
