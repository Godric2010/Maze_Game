#include "Intersects.hpp"
#include <algorithm>
#include <array>
#include <cmath>

#include "Bounds.hpp"
#include "QuaternionMath.hpp"
#include "VectorMath.hpp"

namespace yarep::geometry {
    constexpr float intersection_epsilon = 1e-6f;

    bool intersects(const AABB& lhs, const AABB& rhs) {
        if (lhs.max.x < rhs.min.x || lhs.min.x > rhs.max.x)
            return false;
        if (lhs.max.y < rhs.min.y || lhs.min.y > rhs.max.y)
            return false;
        if (lhs.max.z < rhs.min.z || lhs.min.z > rhs.max.z)
            return false;
        return true;
    }

    bool intersects(const Sphere& lhs, const Sphere& rhs) {
        const auto distance = math::distance_squared(rhs.center, lhs.center);
        return distance <= (lhs.radius + rhs.radius) * (lhs.radius + rhs.radius);
    }

    bool intersects(const Sphere& sphere, const AABB& aabb) {
        const auto closest_point = math::Vec3{
            std::clamp(sphere.center.x, aabb.min.x, aabb.max.x),
            std::clamp(sphere.center.y, aabb.min.y, aabb.max.y),
            std::clamp(sphere.center.z, aabb.min.z, aabb.max.z)
        };
        const auto distance_sqrd = math::distance_squared(closest_point, sphere.center);
        return distance_sqrd <= sphere.radius * sphere.radius;
    }

    bool intersects(const AABB& aabb, const Sphere& sphere) {
        return intersects(sphere, aabb);
    }

    bool intersects(const Sphere& sphere, const OBB& obb) {
        const auto local_point = math::rotate(math::inverse(obb.rotation), sphere.center - obb.center);
        const auto local_aabb = AABB{-obb.half_extents, obb.half_extents};
        const auto closest_point = math::Vec3{
            std::clamp(local_point.x, local_aabb.min.x, local_aabb.max.x),
            std::clamp(local_point.y, local_aabb.min.y, local_aabb.max.y),
            std::clamp(local_point.z, local_aabb.min.z, local_aabb.max.z)
        };

        const auto distance_sqrd = math::distance_squared(closest_point, local_point);
        const auto radius_sqrd = sphere.radius * sphere.radius;
        const auto delta = radius_sqrd - distance_sqrd;
        return delta > -intersection_epsilon;
    }

    bool intersects(const OBB& obb, const Sphere& sphere) {
        return intersects(sphere, obb);
    }

    bool intersects(const OBB& obb, const AABB& aabb) {
        const auto identity_obb = to_obb(aabb, math::Transform());
        return intersects(obb, identity_obb);
    }

    bool intersects(const AABB& aabb, const OBB& obb) {
        return intersects(obb, aabb);
    }

    bool intersects(const OBB& lhs, const OBB& rhs) {
        const std::array lhs_axes = {
            math::rotate(lhs.rotation, {1, 0, 0}),
            math::rotate(lhs.rotation, {0, 1, 0}),
            math::rotate(lhs.rotation, {0, 0, 1})
        };

        const std::array rhs_axes = {
            math::rotate(rhs.rotation, {1, 0, 0}),
            math::rotate(rhs.rotation, {0, 1, 0}),
            math::rotate(rhs.rotation, {0, 0, 1})
        };

        const auto center_delta = rhs.center - lhs.center;
        const auto overlaps_on_axis = [&](const math::Vec3& axis) {
            const auto lhs_radius = std::abs(math::dot(axis, lhs_axes[0])) * lhs.half_extents.x +
                                    std::abs(math::dot(axis, lhs_axes[1])) * lhs.half_extents.y +
                                    std::abs(math::dot(axis, lhs_axes[2])) * lhs.half_extents.z;
            const auto rhs_radius = std::abs(math::dot(axis, rhs_axes[0])) * rhs.half_extents.x +
                                    std::abs(math::dot(axis, rhs_axes[1])) * rhs.half_extents.y +
                                    std::abs(math::dot(axis, rhs_axes[2])) * rhs.half_extents.z;

            const auto center_distance = std::abs(math::dot(center_delta, axis));
            return center_distance <= lhs_radius + rhs_radius;
        };

        for (const auto& axis: lhs_axes) {
            if (!overlaps_on_axis(axis)) {
                return false;
            }
        }
        for (const auto& axis: rhs_axes) {
            if (!overlaps_on_axis(axis)) {
                return false;
            }
        }

        constexpr float axis_epsilon_squared = 1e-8f;

        for (const auto& lhs_axis: lhs_axes) {
            for (const auto& rhs_axis: rhs_axes) {
                auto cross_axis = math::cross(lhs_axis, rhs_axis);
                if (math::length_squared(cross_axis) <= axis_epsilon_squared) {
                    continue;
                }
                cross_axis = math::normalize(cross_axis);
                if (!overlaps_on_axis(cross_axis)) {
                    return false;
                }
            }
        }
        return true;
    }

    bool intersects(const Plane& plane, const Sphere& sphere) {
        const auto sphere_distance_to_plane = std::abs(math::dot(plane.normal, sphere.center) + plane.distance);
        return sphere_distance_to_plane <= sphere.radius;
    }

    bool intersects(const Sphere& sphere, const Plane& plane) {
        return intersects(plane, sphere);
    }

    bool intersects(const Plane& plane, const AABB& aabb) {
        const auto aabb_center = (aabb.min + aabb.max) * 0.5f;
        const auto aabb_half_extents = (aabb.max - aabb.min) * 0.5f;

        const auto aabb_radius = std::abs(math::dot(plane.normal, {1, 0, 0})) * aabb_half_extents.x +
                                 std::abs(math::dot(plane.normal, {0, 1, 0})) * aabb_half_extents.y +
                                 std::abs(math::dot(plane.normal, {0, 0, 1})) * aabb_half_extents.z;

        const auto aabb_center_dist_to_plane = std::abs(math::dot(plane.normal, aabb_center) + plane.distance);
        return aabb_radius >= aabb_center_dist_to_plane;
    }


    bool intersects(const AABB& aabb, const Plane& plane) {
        return intersects(plane, aabb);
    }

    bool intersects(const Plane& plane, const OBB& obb) {
        const std::array obb_axes = {
            math::rotate(obb.rotation, {1, 0, 0}),
            math::rotate(obb.rotation, {0, 1, 0}),
            math::rotate(obb.rotation, {0, 0, 1})
        };
        const auto obb_radius = std::abs(math::dot(plane.normal, obb_axes[0])) * obb.half_extents.x +
                                std::abs(math::dot(plane.normal, obb_axes[1])) * obb.half_extents.y +
                                std::abs(math::dot(plane.normal, obb_axes[2])) * obb.half_extents.z;

        const auto obb_center_dist_to_plane = std::abs(math::dot(plane.normal, obb.center) + plane.distance);
        const auto delta = obb_radius - obb_center_dist_to_plane;
        return delta > -intersection_epsilon;
    }

    bool intersects(const OBB& obb, const Plane& plane) {
        return intersects(plane, obb);
    }
}
