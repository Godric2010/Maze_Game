#include "Contains.hpp"

#include "QuaternionMath.hpp"
#include "VectorMath.hpp"

namespace yarep::geometry {
    bool contains(const AABB& aabb, const math::Vec3& point) {
        const bool bigger_equals_min = aabb.min.x <= point.x &&
                                       aabb.min.y <= point.y &&
                                       aabb.min.z <= point.z;
        const bool smaller_equals_max = aabb.max.x >= point.x &&
                                        aabb.max.y >= point.y &&
                                        aabb.max.z >= point.z;
        return bigger_equals_min && smaller_equals_max;
    }

    bool contains(const Sphere& sphere, const math::Vec3& point) {
        const auto center = sphere.center;
        const auto distance = math::distance_squared(center, point);
        return distance <= sphere.radius * sphere.radius;
    }

    bool contains(const OBB& obb, const math::Vec3& point) {
        auto relative_point = point - obb.center;
        relative_point = math::rotate(math::inverse(obb.rotation), relative_point);

        const bool smaller_than_extents_x = std::abs(relative_point.x) <= obb.half_extents.x;
        const bool smaller_than_extents_y = std::abs(relative_point.y) <= obb.half_extents.y;
        const bool smaller_than_extents_z = std::abs(relative_point.z) <= obb.half_extents.z;
        return smaller_than_extents_x && smaller_than_extents_y && smaller_than_extents_z;
    }
}
