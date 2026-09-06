#pragma once
#include "Angle.hpp"
#include "Mat3.hpp"
#include "Mat4.hpp"
#include "Quaternion.hpp"
#include "Vec3.hpp"

namespace yarep::math {
    float length_squared(const Quaternion& quaternion);

    float length(const Quaternion& quaternion);

    Quaternion normalize(const Quaternion& quaternion);

    float dot(const Quaternion& lhs, const Quaternion& rhs);

    Quaternion conjugate(const Quaternion& quaternion);

    Quaternion inverse(const Quaternion& quaternion);

    Quaternion from_axis_angle(const Vec3& axis, const Angle& angle);

    Vec3 rotate(const Quaternion& quaternion, const Vec3& vector);

    Mat3 to_mat3(const Quaternion& quaternion);

    Mat4 to_mat4(const Quaternion& quaternion);

    Quaternion slerp(const Quaternion& lhs, const Quaternion& rhs, float t);

    bool nearly_equal(const Quaternion& lhs, const Quaternion& rhs, float tolerance);
}
