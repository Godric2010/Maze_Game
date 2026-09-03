#pragma once
#include <array>
#include <cstddef>
#include "Vec3.hpp"

namespace yarep::math {
    struct Mat3 {
        std::array<float, 9> values{};

        constexpr Mat3() = default;

        constexpr Mat3(const Vec3& column0, const Vec3& column1, const Vec3& column2) : values{
            column0.x,
            column0.y,
            column0.z,
            column1.x,
            column1.y,
            column1.z,
            column2.x,
            column2.y,
            column2.z,
        } {
        }

        static constexpr Mat3 identity() {
            return Mat3{
                Vec3{1.0f, 0.0f, 0.0f},
                Vec3{0.0f, 1.0f, 0.0f},
                Vec3{0.0f, 0.0f, 1.0f},
            };
        }

        constexpr float &operator()(std::size_t row, std::size_t column) {
            return values[column * 3 + row];
        }

        constexpr const float &operator()(std::size_t row, std::size_t column) const {
            return values[column * 3 + row];
        }

        constexpr bool operator==(const Mat3&) const = default;
    };

    constexpr Vec3 operator*(const Mat3& mat, const Vec3& vec) {
        return Vec3{
            mat(0, 0) * vec.x +
            mat(0, 1) * vec.y +
            mat(0, 2) * vec.z,

            mat(1, 0) * vec.x +
            mat(1, 1) * vec.y +
            mat(1, 2) * vec.z,

            mat(2, 0) * vec.x +
            mat(2, 1) * vec.y +
            mat(2, 2) * vec.z,
        };
    }

    constexpr Mat3 operator*(const Mat3& lhs, const Mat3& rhs) {
        Mat3 result;
        for (std::size_t row = 0; row < 3; ++row) {
            for (std::size_t column = 0; column < 3; ++column) {
                for (std::size_t i = 0; i < 3; ++i) {
                    result(row, column) += lhs(row, i) * rhs(i, column);
                }
            }
        }
        return result;
    }
}
