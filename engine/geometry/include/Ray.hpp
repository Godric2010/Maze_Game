#pragma once
#include <Vec3.hpp>

namespace yarep::geometry {
    /**
    * @struct Ray
    * @brief Represents a 3D ray in space.
    *
    * A ray is defined as:
    *
    *     P(t) = origin + direction * t
    *
    * The direction vector is stored unchanged and is not required to be normalized.
    * A zero direction is invalid for geometric intersection queries.
    * @note The direction vector is typically expected to be normalized.
    */
    struct Ray {
        math::Vec3 origin{};
        math::Vec3 direction{};

        constexpr Ray() = default;
        constexpr Ray(math::Vec3 origin, math::Vec3 direction): origin(origin), direction(direction) {}

        constexpr bool operator==(const Ray &other) const {
            return origin == other.origin && direction == other.direction;
        }
    };
}