#pragma once
#include <stdexcept>

namespace yarep::math {
    struct Vec4 {
        float x = 0.0f;
        float y = 0.0f;
        float z = 0.0f;
        float w = 0.0f;

        constexpr Vec4() = default;

        constexpr Vec4(const float x, const float y, const float z, const float w) : x(x), y(y), z(z), w(w) {
        }

        constexpr Vec4 operator+(const Vec4& other) const {
            return {x + other.x, y + other.y, z + other.z, w + other.w};
        }

        constexpr Vec4 operator-(const Vec4& other) const {
            return {x - other.x, y - other.y, z - other.z, w - other.w};
        }

        constexpr Vec4 operator+=(const Vec4& other) {
            x += other.x;
            y += other.y;
            z += other.z;
            w += other.w;
            return *this;
        }

        constexpr Vec4 operator-=(const Vec4& other) {
            x -= other.x;
            y -= other.y;
            z -= other.z;
            w -= other.w;
            return *this;
        }

        constexpr Vec4 operator*(const float scalar) const {
            return {x * scalar, y * scalar, z * scalar, w * scalar};
        }

        constexpr Vec4 operator/(const float scalar) const {
            if (scalar == 0.0f) {
                throw std::runtime_error("Cannot divide by zero");
            }
            return {x / scalar, y / scalar, z / scalar, w / scalar};
        }

        constexpr Vec4 operator*=(const float scalar) {
            x *= scalar;
            y *= scalar;
            z *= scalar;
            w *= scalar;
            return *this;
        }

        constexpr Vec4 operator/=(const float scalar) {
            if (scalar == 0.0f) {
                throw std::runtime_error("Cannot divide by zero");
            }

            x /= scalar;
            y /= scalar;
            z /= scalar;
            w /= scalar;
            return *this;
        }

        constexpr Vec4 operator-() const {
            return {-x, -y, -z, -w};
        }

        constexpr bool operator==(const Vec4& other) const {
            return x == other.x && y == other.y && z == other.z && w == other.w;
        }
    };

    constexpr Vec4 operator*(const float scalar, const Vec4& vec) {
        return vec * scalar;
    }
}
