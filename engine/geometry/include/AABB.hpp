#pragma once
#include <Vec3.hpp>

namespace yarep::geometry {
    // ReSharper disable once CppInconsistentNaming
    /**
     * @struct AABB
     * @brief Represents an axis-aligned bounding box in a 3D space.
     *
     * The AABB structure is defined by two points, `min` and `max`, which represent
     * the minimum and maximum corners of the box respectively. It is commonly used
     * in spatial partitioning, collision detection, and other physics-related
     * calculations.
     *
     * @note The components of `min` should be less than or equal to the corresponding
     * components of `max`.
     */
    struct AABB {
        math::Vec3 min{};
        math::Vec3 max{};

        constexpr AABB() = default;

        constexpr AABB(const math::Vec3& min, const math::Vec3& max) : min(min), max(max) {
        }

        constexpr bool operator==(const AABB& other) const {
            return min == other.min && max == other.max;
        }
    };
}
