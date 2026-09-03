#pragma once
#include <array>
#include <cstddef>
#include "Vec4.hpp"

namespace yarep::math {
    struct Mat4 {
        std::array<float, 16> values{};

        constexpr Mat4() = default;

        constexpr Mat4(const Vec4& column0, const Vec4& column1, const Vec4& column2, const Vec4& column3) : values{
            column0.x,
            column0.y,
            column0.z,
            column0.w,
            column1.x,
            column1.y,
            column1.z,
            column1.w,
            column2.x,
            column2.y,
            column2.z,
            column2.w,
            column3.x,
            column3.y,
            column3.z,
            column3.w,
        } {
        }

        static constexpr Mat4 identity() {
            return Mat4{
                Vec4{1.0f, 0.0f, 0.0f, 0.0f},
                Vec4{0.0f, 1.0f, 0.0f, 0.0f},
                Vec4{0.0f, 0.0f, 1.0f, 0.0f},
                Vec4{0.0f, 0.0f, 0.0f, 1.0f},
            };
        }

        constexpr float &operator()(std::size_t row, std::size_t column) {
            return values[column * 4 + row];
        }

        constexpr const float &operator()(std::size_t row, std::size_t column) const {
            return values[column * 4 + row];
        }

        constexpr bool operator==(const Mat4&) const = default;
    };

    constexpr Vec4 operator*(const Mat4& mat, const Vec4& vec) {
        return Vec4{
            mat(0, 0) * vec.x +
            mat(0, 1) * vec.y +
            mat(0, 2) * vec.z +
            mat(0, 3) * vec.w,

            mat(1, 0) * vec.x +
            mat(1, 1) * vec.y +
            mat(1, 2) * vec.z +
            mat(1, 3) * vec.w,

            mat(2, 0) * vec.x +
            mat(2, 1) * vec.y +
            mat(2, 2) * vec.z +
            mat(2, 3) * vec.w,

            mat(3, 0) * vec.x +
            mat(3, 1) * vec.y +
            mat(3, 2) * vec.z +
            mat(3, 3) * vec.w,
        };
    }

    constexpr Mat4 operator*(const Mat4& lhs, const Mat4& rhs) {
        Mat4 result;
        for (std::size_t row = 0; row < 4; ++row) {
            for (std::size_t column = 0; column < 4; ++column) {
                for (std::size_t i = 0; i < 4; ++i) {
                    result(row, column) += lhs(row, i) * rhs(i, column);
                }
            }
        }
        return result;
    }
}
