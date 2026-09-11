#include "ClosestPoint.hpp"

#include <algorithm>
#include "QuaternionMath.hpp"
#include "VectorMath.hpp"

namespace yarep::geometry {
    math::Vec3 closest_point(const Sphere& sphere, const math::Vec3& point) {
        if (math::distance_squared(sphere.center, point) > sphere.radius * sphere.radius) {
            return math::normalize(point - sphere.center) * sphere.radius + sphere.center;
        }
        return point;
    }

    math::Vec3 closest_point(const Plane& plane, const math::Vec3& point) {
        const auto point_dist_to_plane = math::dot(plane.normal, point) + plane.distance;
        return point - plane.normal * point_dist_to_plane;
    }

    math::Vec3 closest_point(const AABB& aabb, const math::Vec3& point) {
        return {
            std::clamp(point.x, aabb.min.x, aabb.max.x),
            std::clamp(point.y, aabb.min.y, aabb.max.y),
            std::clamp(point.z, aabb.min.z, aabb.max.z)
        };
    }

    math::Vec3 closest_point(const OBB& obb, const math::Vec3& point) {
        const auto point_in_obb_space = math::rotate(math::inverse(obb.rotation), point - obb.center);
        const auto clamped_point = math::Vec3{
            std::clamp(point_in_obb_space.x, -obb.half_extents.x, obb.half_extents.x),
            std::clamp(point_in_obb_space.y, -obb.half_extents.y, obb.half_extents.y),
            std::clamp(point_in_obb_space.z, -obb.half_extents.z, obb.half_extents.z)
        };

        const auto rotated_clamped_point = math::rotate(obb.rotation, clamped_point);
        return obb.center + rotated_clamped_point;
    }
}
