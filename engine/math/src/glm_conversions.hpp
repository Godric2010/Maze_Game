#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include "Mat3.hpp"
#include "Mat4.hpp"
#include "Quaternion.hpp"
#include "Vec3.hpp"
#include "Vec4.hpp"

namespace yarep::math::detail {
    static glm::vec3 to_glm_vec3(const Vec3& vec) {
        return glm::vec3{vec.x, vec.y, vec.z};
    }

    static Vec3 from_glm_vec3(const glm::vec3& vec) {
        return Vec3{vec.x, vec.y, vec.z};
    }

    static glm::mat3 to_glm_mat3(const Mat3& mat) {
        return glm::mat3{
            glm::vec3{mat(0, 0), mat(1, 0), mat(2, 0)},
            glm::vec3{mat(0, 1), mat(1, 1), mat(2, 1)},
            glm::vec3{mat(0, 2), mat(1, 2), mat(2, 2)},
        };
    }

    static Mat3 from_glm_mat3(const glm::mat3& mat) {
        return Mat3{
            {mat[0][0], mat[0][1], mat[0][2]},
            {mat[1][0], mat[1][1], mat[1][2]},
            {mat[2][0], mat[2][1], mat[2][2]}
        };
    }

    static glm::mat4 to_glm_mat4(const Mat4& mat) {
        return glm::mat4{
            glm::vec4{mat(0, 0), mat(1, 0), mat(2, 0), mat(3, 0)},
            glm::vec4{mat(0, 1), mat(1, 1), mat(2, 1), mat(3, 1)},
            glm::vec4{mat(0, 2), mat(1, 2), mat(2, 2), mat(3, 2)},
            glm::vec4{mat(0, 3), mat(1, 3), mat(2, 3), mat(3, 3)},
        };
    }

    static Mat4 from_glm_mat4(const glm::mat4& mat) {
        return Mat4{
            {mat[0][0], mat[0][1], mat[0][2], mat[0][3]},
            {mat[1][0], mat[1][1], mat[1][2], mat[1][3]},
            {mat[2][0], mat[2][1], mat[2][2], mat[2][3]},
            {mat[3][0], mat[3][1], mat[3][2], mat[3][3]}
        };
    }

    static glm::quat to_glm_quat(const Quaternion& quaternion) {
        return glm::quat{
            quaternion.w,
            quaternion.x,
            quaternion.y,
            quaternion.z
        };
    }

    static Quaternion from_glm_quat(const glm::quat& quaternion) {
        return Quaternion{
            quaternion.x,
            quaternion.y,
            quaternion.z,
            quaternion.w
        };
    }
}
