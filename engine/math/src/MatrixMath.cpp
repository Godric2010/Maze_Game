#include "MatrixMath.hpp"
#include <cmath>
#include <cstddef>
#include <stdexcept>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

#include "Constants.hpp"
#include "VectorMath.hpp"


namespace yarep::math {
    static glm::vec3 to_glm_vec3(const Vec3& vec) {
        return glm::vec3{vec.x, vec.y, vec.z};
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

    Mat3 transpose(const Mat3& matrix) {
        const glm::mat3 glm_mat = to_glm_mat3(matrix);
        const auto transposed_glm_mat = glm::transpose(glm_mat);
        return from_glm_mat3(transposed_glm_mat);
    }

    Mat4 transpose(const Mat4& matrix) {
        const glm::mat4 glm_mat = to_glm_mat4(matrix);
        const auto transposed_glm_mat = glm::transpose(glm_mat);
        return from_glm_mat4(transposed_glm_mat);
    }

    float determinant(const Mat3& matrix) {
        const glm::mat3 glm_mat = to_glm_mat3(matrix);
        return glm::determinant(glm_mat);
    }

    float determinant(const Mat4& matrix) {
        const glm::mat4 glm_mat = to_glm_mat4(matrix);
        return glm::determinant(glm_mat);
    }

    Mat3 inverse(const Mat3& matrix) {
        const glm::mat3 glm_mat = to_glm_mat3(matrix);
        const float det = glm::determinant(glm_mat);
        if (std::abs(det) == 0.0f) {
            throw std::runtime_error("Cannot invert singular matrix");
        }
        const auto inverse_glm_mat = glm::inverse(glm_mat);
        return from_glm_mat3(inverse_glm_mat);
    }

    Mat4 inverse(const Mat4& matrix) {
        const glm::mat4 glm_mat = to_glm_mat4(matrix);
        const float det = glm::determinant(glm_mat);
        if (std::abs(det) == 0.0f) {
            throw std::runtime_error("Cannot invert singular matrix");
        }
        const auto inverse_glm_mat = glm::inverse(glm_mat);
        return from_glm_mat4(inverse_glm_mat);
    }

    bool nearly_equal(const Mat3& lhs, const Mat3& rhs, const float epsilon) {
        for (std::size_t row = 0; row < 3; ++row) {
            for (std::size_t col = 0; col < 3; ++col) {
                if (std::abs(lhs(row, col) - rhs(row, col)) > epsilon) {
                    return false;
                }
            }
        }
        return true;
    }

    bool nearly_equal(const Mat4& lhs, const Mat4& rhs, float epsilon) {
        for (std::size_t row = 0; row < 4; ++row) {
            for (std::size_t col = 0; col < 4; ++col) {
                if (std::abs(lhs(row, col) - rhs(row, col)) > epsilon) {
                    return false;
                }
            }
        }
        return true;
    }

    Mat4 translation_matrix(const Vec3& translation) {
        const auto translation_glm_vec = to_glm_vec3(translation);
        const auto translate_glm_mat = glm::translate(glm::mat4(1.0f), translation_glm_vec);
        return from_glm_mat4(translate_glm_mat);
    }

    Mat4 scale_matrix(const Vec3& scale) {
        const auto scale_glm_vec = to_glm_vec3(scale);
        const auto scale_glm_mat = glm::scale(glm::mat4(1.0f), scale_glm_vec);
        return from_glm_mat4(scale_glm_mat);
    }

    Mat4 rotation_matrix(const Quaternion& rotation) {
        const auto rotation_glm_quat = to_glm_quat(rotation);
        const auto mat_glm = glm::mat4_cast(rotation_glm_quat);
        return from_glm_mat4(mat_glm);
    }

    Mat4 trs_matrix(const Vec3& translation, const Quaternion& rotation, const Vec3& scale) {
        return translation_matrix(translation) * rotation_matrix(rotation) * scale_matrix(scale);
    }

    Mat4 look_at(const Vec3& eye, const Vec3& target, const Vec3& up) {
        if (eye == target) {
            throw std::runtime_error("Cannot look at target, since eye is equal to target");
        }
        if (up == Vec3{0, 0, 0}) {
            throw std::runtime_error("Cannot look at, since up vector is zero");
        }

        const auto normalized_up = normalize(up);
        const auto look_direction = normalize(eye - target);
        const auto side = cross(look_direction, normalized_up);
        if (length_squared(side) <= EPSILON * EPSILON) {
            throw std::runtime_error("Cannot look at direction, since up vector is perpendicular to target look vector"
                    );
        }

        const auto eye_glm_vec = to_glm_vec3(eye);
        const auto target_glm_vec = to_glm_vec3(target);
        const auto up_glm_vec = to_glm_vec3(up);
        const auto look_at_glm = glm::lookAtRH(eye_glm_vec, target_glm_vec, up_glm_vec);
        return from_glm_mat4(look_at_glm);
    }

    Mat4 perspective(Angle vertical_fov, float aspect_ratio, float near_plane, float far_plane) {
        if (vertical_fov.radians() <= 0.0f) {
            throw std::runtime_error("Cannot perspective projection, since fov is smaller-equals zero");
        }
        if (vertical_fov.radians() >= PI) {
            throw std::runtime_error("Cannot perspective projection, since fov is larger-equals 180 deg");
        }
        if (aspect_ratio <= 0.0f) {
            throw std::runtime_error("Cannot perspective projection, since aspect ratio is smaller-equals zero");
        }
        if (near_plane <= 0.0f) {
            throw std::runtime_error("Cannot perspective projection, since near_plane is smaller-equals zero");
        }
        if (far_plane <= near_plane) {
            throw std::runtime_error("Cannot perspective projection, since near_plane is larger-equals far plane");
        }

        const auto perspective_glm = glm::perspectiveRH_NO(vertical_fov.radians(),
                                                           aspect_ratio,
                                                           near_plane,
                                                           far_plane

                );
        return from_glm_mat4(perspective_glm);
    }

    Mat4 orthographic(float left, float right, float bottom, float top, float near_plane, float far_plane) {
        if (left == right) {
            throw std::runtime_error("Cannot orthographic projection, since left equals right");
        }
        if (top == bottom) {
            throw std::runtime_error("Cannot orthographic projection, since top equals bottom");
        }
        if (near_plane == far_plane) {
            throw std::runtime_error("Cannot orthographic projection, since near equals far");
        }
        const auto ortho_glm = glm::orthoRH_NO(left, right, bottom, top, near_plane, far_plane);
        return from_glm_mat4(ortho_glm);
    }

    Mat3 normal_matrix(const Mat4& model_matrix) {
        const auto model_matrix_glm = to_glm_mat4(model_matrix);
        const auto linear_matrix_glm = glm::mat3(model_matrix_glm);
        const float det = glm::determinant(linear_matrix_glm);
        if (std::abs(det) <= EPSILON) {
            throw std::runtime_error("Cannot invert singular matrix");
        }
        const auto normal_glm_mat = glm::transpose(glm::inverse(linear_matrix_glm));
        return from_glm_mat3(normal_glm_mat);
    }
}
