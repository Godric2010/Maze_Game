#include "TransformMath.hpp"
#include "MatrixMath.hpp"
#include "QuaternionMath.hpp"
#include "VectorMath.hpp"

namespace yarep::math {
    Mat4 to_matrix(const Transform& transform) {
        return trs_matrix(transform.position, transform.rotation, transform.scale);
    }

    Vec3 transform_point(const Transform& transform, const Vec3& point) {
        const auto transform_matrix = to_matrix(transform);
        const auto transformed_point = transform_matrix * Vec4{point.x, point.y, point.z, 1.0f};
        return Vec3{transformed_point.x, transformed_point.y, transformed_point.z};
    }

    Vec3 transform_vector(const Transform& transform, const Vec3& vector) {
        const auto transform_matrix = to_matrix(transform);
        const auto transformed_vector = transform_matrix * Vec4{vector.x, vector.y, vector.z, 0.0f};
        return Vec3{transformed_vector.x, transformed_vector.y, transformed_vector.z};
    }

    Vec3 transform_direction(const Transform& transform, const Vec3& direction) {
        return rotate(transform.rotation, direction);
    }

    Vec3 forward(const Transform& transform) {
        return rotate(transform.rotation, Vec3{0, 0, -1});
    }

    Vec3 right(const Transform& transform) {
        return rotate(transform.rotation, Vec3{1, 0, 0});
    }

    Vec3 up(const Transform& transform) {
        return rotate(transform.rotation, Vec3{0, 1, 0});
    }

    bool nearly_equal(const Transform& a, const Transform& b, float tolerance) {
        const auto position_nearly_equal = nearly_equal(a.position, b.position, tolerance);
        const auto rotation_nearly_equal = nearly_equal(a.rotation, b.rotation, tolerance);
        const auto scale_nearly_equal = nearly_equal(a.scale, b.scale, tolerance);
        return position_nearly_equal && rotation_nearly_equal && scale_nearly_equal;
    }
}
