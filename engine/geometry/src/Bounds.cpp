#include "Bounds.hpp"

#include <algorithm>
#include <stdexcept>

#include "MatrixMath.hpp"
#include "QuaternionMath.hpp"
#include "TransformMath.hpp"
#include "VectorMath.hpp"

namespace yarep::geometry {
    AABB to_aabb(const Sphere &sphere) {
        if (sphere.radius < 0) {
            throw std::invalid_argument("A sphere radius must be greater than zero to from a valid AABB");
        }
        const math::Vec3 r = {sphere.radius, sphere.radius, sphere.radius};
        const math::Vec3 min = sphere.center - r;
        const math::Vec3 max = sphere.center + r;
        return {min, max};
    }

    AABB to_aabb(const OBB &obb) {
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

    OBB to_obb(const AABB &aabb, const math::Transform &transform) {
        const math::Vec3 half_extents = {
            (aabb.max.x - aabb.min.x) * 0.5f,
            (aabb.max.y - aabb.min.y) * 0.5f,
            (aabb.max.z - aabb.min.z) * 0.5f,
        };

        const auto local_center = aabb.min + half_extents;
        const auto local_center_vec4 = math::Vec4(local_center.x, local_center.y, local_center.z, 1);
        const auto trs_matrix = math::to_matrix(transform);
        const auto center_vec4 = trs_matrix * local_center_vec4;
        const auto center = math::Vec3(center_vec4.x, center_vec4.y, center_vec4.z);

        // Scale obb half-extents
        const auto scaled_half_extents = math::Vec3{
            half_extents.x * std::abs(transform.scale.x),
            half_extents.y * std::abs(transform.scale.y),
            half_extents.z * std::abs(transform.scale.z)
        };

        return OBB{center, scaled_half_extents, transform.rotation};
    }

    AABB transform_bounds(const AABB &aabb, const math::Transform &transform) {
        return transform_bounds(aabb, math::to_matrix(transform));
    }

    AABB transform_bounds(const AABB &aabb, const math::Mat4 &matrix) {
        // local center and half-extents
        const auto center = geometry::center(aabb);
        const auto half_extents = geometry::half_extents(aabb);

        // Transform center according to matrix
        const auto transformed_center_vec4 = matrix * math::Vec4(center.x, center.y, center.z, 1);
        const auto transformed_center = math::Vec3(transformed_center_vec4.x, transformed_center_vec4.y,
                                                   transformed_center_vec4.z);

        // Transform local basis vectors (without translation)
        const auto axis_x = matrix * math::Vec4(1, 0, 0, 0);
        const auto axis_y = matrix * math::Vec4(0, 1, 0, 0);
        const auto axis_z = matrix * math::Vec4(0, 0, 1, 0);

        // Calculate half-extents according to matrix
        const math::Vec3 transformed_half_extents{
            std::abs(axis_x.x) * half_extents.x +
            std::abs(axis_y.x) * half_extents.y +
            std::abs(axis_z.x) * half_extents.z,
            std::abs(axis_x.y) * half_extents.x +
            std::abs(axis_y.y) * half_extents.y +
            std::abs(axis_z.y) *
            half_extents.z,
            std::abs(axis_x.z) * half_extents.x +
            std::abs(axis_y.z) * half_extents.y +
            std::abs(axis_z.z) * half_extents.z
        };

        return AABB{
            transformed_center - transformed_half_extents,
            transformed_center + transformed_half_extents,
        };
    }

    AABB expand(const AABB &aabb, float amount) {
        const math::Vec3 expansion{amount, amount, amount};
        return {aabb.min - expansion, aabb.max + expansion};
    }

    OBB expand(const OBB &obb, float amount) {
        const math::Vec3 expansion{amount, amount, amount};
        return {
            obb.center,
            obb.half_extents + expansion,
            obb.rotation
        };
    }

    AABB merge(const AABB &a, const AABB &b) {
        return {
            math::min(a.min, b.min),
            math::max(a.max, b.max),
        };
    }
}
