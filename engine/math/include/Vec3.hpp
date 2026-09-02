#pragma once
#include <stdexcept>

namespace yarep::math {
    struct Vec3 {
        float x = 0.0f;
        float y = 0.0f;
        float z = 0.0f;

        constexpr Vec3() = default;

        constexpr Vec3(const float x, const float y, const float z) : x(x), y(y), z(z) {
        }

        constexpr Vec3 operator+(const Vec3& other) const {
            return {x + other.x, y + other.y, z + other.z};
        }

        constexpr Vec3 operator-(const Vec3& other) const {
            return {x - other.x, y - other.y, z - other.z};
        }

        constexpr Vec3 operator+=(const Vec3& other) {
            x += other.x;
            y += other.y;
            z += other.z;
            return *this;
        }

        constexpr Vec3 operator-=(const Vec3& other) {
            x -= other.x;
            y -= other.y;
            z -= other.z;
            return *this;
        }

        constexpr Vec3 operator*(const float scalar) const {
            return {x * scalar, y * scalar, z * scalar};
        }

        constexpr Vec3 operator/(const float scalar) const {
            if (scalar == 0.0f) {
                throw std::runtime_error("Cannot divide by zero");
            }
            return {x / scalar, y / scalar, z / scalar};
        }

        constexpr Vec3 operator*=(const float scalar) {
            x *= scalar;
            y *= scalar;
            z *= scalar;
            return *this;
        }

        constexpr Vec3 operator/=(const float scalar) {
            if (scalar == 0.0f) {
                throw std::runtime_error("Cannot divide by zero");
            }

            x /= scalar;
            y /= scalar;
            z /= scalar;
            return *this;
        }

        constexpr Vec3 operator-() const {
            return {-x, -y, -z};
        }

        constexpr bool operator==(const Vec3& other) const {
            return x == other.x && y == other.y && z == other.z;
        }
    };

    constexpr Vec3 operator*(const float scalar, const Vec3& vec) {
        return vec * scalar;
    }
}
