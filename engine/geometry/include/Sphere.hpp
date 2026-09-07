#pragma once
#include <Vec3.hpp>

namespace yarep::geometry {
    /**
        * @struct Sphere
        * @brief Represents a 3D geometric sphere.
        *
        * The Sphere structure is defined by a center point in 3D space and a radius.
        * It is widely used in physics simulations, collision detection, bounding volume
        * hierarchies, and graphical rendering.
        *
        * @note The radius is expected to be non-negative.
        */
    struct Sphere {
        math::Vec3 center{};
        float radius = 0.0f;

        constexpr Sphere() = default;
        constexpr Sphere(const math::Vec3& center, float radius) : center(center), radius(radius) {}

        constexpr bool operator==(const Sphere& other) const {
            return center == other.center && radius == other.radius;
        }
    };
}