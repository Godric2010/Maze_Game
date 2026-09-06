#pragma once
#include "Angle.hpp"
#include "Mat3.hpp"
#include "Mat4.hpp"
#include "Quaternion.hpp"
#include "Vec3.hpp"

namespace yarep::math {
    Mat3 transpose(const Mat3& matrix);

    Mat4 transpose(const Mat4& matrix);

    float determinant(const Mat3& matrix);

    float determinant(const Mat4& matrix);

    Mat3 inverse(const Mat3& matrix);

    Mat4 inverse(const Mat4& matrix);

    bool nearly_equal(const Mat3& lhs, const Mat3& rhs, float epsilon);

    bool nearly_equal(const Mat4& lhs, const Mat4& rhs, float epsilon);

    Mat4 translation_matrix(const Vec3& translation);

    Mat4 scale_matrix(const Vec3& scale);

    Mat4 rotation_matrix(const Quaternion& rotation);

    Mat4 trs_matrix(
            const Vec3& translation,
            const Quaternion& rotation,
            const Vec3& scale);

    Mat4 look_at(
            const Vec3& eye,
            const Vec3& target,
            const Vec3& up);

    Mat4 perspective(
            Angle vertical_fov,
            float aspect_ratio,
            float near_plane,
            float far_plane);

    Mat4 orthographic(
            float left,
            float right,
            float bottom,
            float top,
            float near_plane,
            float far_plane);

    Mat3 normal_matrix(const Mat4& model_matrix);
}
