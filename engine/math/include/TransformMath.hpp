#pragma once
#include "Mat4.hpp"
#include "Transform.hpp"

namespace yarep::math {
    Mat4 to_matrix(const Transform& transform);

    Vec3 transform_point(const Transform& transform, const Vec3& point);

    Vec3 transform_vector(const Transform& transform, const Vec3& vector);

    Vec3 transform_direction(const Transform& transform, const Vec3& direction);

    Vec3 forward(const Transform& transform);

    Vec3 up(const Transform& transform);

    Vec3 right(const Transform& transform);

    bool nearly_equal(const Transform& a, const Transform& b, float tolerance);
}
