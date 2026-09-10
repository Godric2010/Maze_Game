#include "Bounds.hpp"

#include <algorithm>
#include <stdexcept>

#include "MatrixMath.hpp"
#include "QuaternionMath.hpp"
#include "TransformMath.hpp"
#include "VectorMath.hpp"

namespace yarep::geometry {
    AABB to_aabb(const Sphere& sphere) {
        if (sphere.radius < 0) {
            throw std::invalid_argument("A sphere radius must be greater than zero to from a valid AABB");
        }
        const math::Vec3 r = {sphere.radius, sphere.radius, sphere.radius};
        const math::Vec3 min = sphere.center - r;
        const math::Vec3 max = sphere.center + r;
        return {min, max};
    }

    AABB to_aabb(const OBB& obb) {
        const auto rotation = obb.rotation;
        const auto rotated_x_axis = math::rotate(rotation, math::Vec3{1, 0, 0});
        const auto rotated_y_axis = math::rotate(rotation, math::Vec3{0, 1, 0});
        const auto rotated_z_axis = math::rotate(rotation, math::Vec3{0, 0, 1});

        const math::Vec3 extent = {
            std::abs(rotated_x_axis.x) * obb.half_extents.x +
            std::abs(rotated_y_axis.x) * obb.half_extents.y +
            std::abs(rotated_z_axis.x) * obb.half_extents.z,
            std::abs(rotated_x_axis.y) * obb.half_extents.x +
            std::abs(rotated_y_axis.y) * obb.half_extents.y +
            std::abs(rotated_z_axis.y) * obb.half_extents.z,
            std::abs(rotated_x_axis.z) * obb.half_extents.x +
            std::abs(rotated_y_axis.z) * obb.half_extents.y +
            std::abs(rotated_z_axis.z) * obb.half_extents.z
        };

        const auto min = obb.center - extent;
        const auto max = obb.center + extent;

        return {min, max};
    }

    OBB to_obb(const AABB& aabb, const math::Transform& transform) {
        const math::Vec3 half_extents = {
            (aabb.max.x - aabb.min.x) * 0.5f,
            (aabb.max.y - aabb.min.y) * 0.5f,
            (aabb.max.z - aabb.min.z) * 0.5f,
        };

        const auto local_center = aabb.min + half_extents;
        const auto local_center_vec4 = math::Vec4(local_center.x, local_center.y, local_center.z, 1);
        const auto trs_matrix = math::to_matrix(transform);
        const auto center_vec4 =  trs_matrix * local_center_vec4;
        const auto center = math::Vec3(center_vec4.x, center_vec4.y, center_vec4.z);

        // Scale obb half-extents
        const auto scaled_half_extents = math::Vec3{
            half_extents.x * std::abs(transform.scale.x),
            half_extents.y * std::abs(transform.scale.y),
            half_extents.z * std::abs(transform.scale.z)
        };

        return OBB{center, scaled_half_extents, transform.rotation};
    }

    AABB transform_bounds(const AABB& aabb, const math::Transform& transform) {
        const auto obb = to_obb(aabb, transform);
        const auto tight_aabb = to_aabb(obb);
        return tight_aabb;
    }
}
