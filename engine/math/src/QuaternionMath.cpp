#include "QuaternionMath.hpp"
#include "AngleMath.hpp"
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

#include "glm_conversions.hpp"
#include "VectorMath.hpp"

namespace yarep::math {
    float length_squared(const Quaternion& quaternion) {
        const auto glm_quat = detail::to_glm_quat(quaternion);
        return glm::length2(glm_quat);
    }

    float length(const Quaternion& quaternion) {
        const auto glm_quat = detail::to_glm_quat(quaternion);
        return glm::length(glm_quat);
    }

    Quaternion normalize(const Quaternion& quaternion) {
        if (length_squared(quaternion) == 0.0f) {
            throw std::runtime_error("Quaternion normalization cannot be zero");
        }
        const auto glm_quat = detail::to_glm_quat(quaternion);
        const auto glm_quat_normalized = glm::normalize(glm_quat);
        return detail::from_glm_quat(glm_quat_normalized);
    }

    float dot(const Quaternion& lhs, const Quaternion& rhs) {
        const auto glm_quat_lhs = detail::to_glm_quat(lhs);
        const auto glm_quat_rhs = detail::to_glm_quat(rhs);
        return glm::dot(glm_quat_lhs, glm_quat_rhs);
    }

    Quaternion conjugate(const Quaternion& quaternion) {
        const auto glm_quat = detail::to_glm_quat(quaternion);
        const auto glm_quat_conjugate = glm::conjugate(glm_quat);
        return detail::from_glm_quat(glm_quat_conjugate);
    }

    Quaternion inverse(const Quaternion& quaternion) {
        if (length_squared(quaternion) == 0.0f) {
            throw std::runtime_error("Quaternion inverse cannot be zero");
        }
        const auto glm_quat = detail::to_glm_quat(quaternion);
        const auto glm_quat_inverse = glm::inverse(glm_quat);
        return detail::from_glm_quat(glm_quat_inverse);
    }

    Quaternion from_axis_angle(const Vec3& axis, const Angle& angle) {
        if (length_squared(axis) == 0.0f) {
            throw std::runtime_error("Axis length cannot be zero");
        }
        const Vec3 axis_norm = normalize(axis);
        const auto glm_axis_norm = detail::to_glm_vec3(axis_norm);
        const auto angle_rad = angle.radians();
        const auto glm_quat = glm::angleAxis(angle_rad, glm_axis_norm);
        return detail::from_glm_quat(glm_quat);
    }

    Vec3 rotate(const Quaternion& quaternion, const Vec3& vector) {
        const auto glm_quat = detail::to_glm_quat(quaternion);
        const auto glm_vec3 = detail::to_glm_vec3(vector);
        const auto rotated_glm_vec3 = glm::rotate(glm_quat, glm_vec3);
        return detail::from_glm_vec3(rotated_glm_vec3);
    }

    Mat3 to_mat3(const Quaternion& quaternion) {
        const auto glm_quat = detail::to_glm_quat(quaternion);
        const auto glm_mat3 = glm::toMat3(glm_quat);
        return detail::from_glm_mat3(glm_mat3);
    }

    Mat4 to_mat4(const Quaternion& quaternion) {
        const auto glm_quat = detail::to_glm_quat(quaternion);
        const auto glm_mat4 = glm::toMat4(glm_quat);
        return detail::from_glm_mat4(glm_mat4);
    }

    Quaternion slerp(const Quaternion& lhs, const Quaternion& rhs, float t) {
        const auto glm_quat_lhs = detail::to_glm_quat(lhs);
        const auto glm_quat_rhs = detail::to_glm_quat(rhs);
        const auto slerped_glm_quat = glm::slerp(glm_quat_lhs, glm_quat_rhs, t);
        return detail::from_glm_quat(slerped_glm_quat);
    }

    bool nearly_equal(const Quaternion& lhs, const Quaternion& rhs, float tolerance) {
        const bool same =
                std::abs(lhs.x - rhs.x) < tolerance &&
                std::abs(lhs.y - rhs.y) < tolerance &&
                std::abs(lhs.z - rhs.z) < tolerance &&
                std::abs(lhs.w - rhs.w) < tolerance;
        const bool negated =
                std::abs(lhs.x + rhs.x) < tolerance &&
                std::abs(lhs.y + rhs.y) < tolerance &&
                std::abs(lhs.z + rhs.z) < tolerance &&
                std::abs(lhs.w + rhs.w) < tolerance;
        return same || negated;
    }
}
