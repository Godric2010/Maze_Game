#pragma once
#include <stdexcept>

namespace yarep::math {
    struct Vec2 {
        float x = 0.0f;
        float y = 0.0f;

        constexpr Vec2() = default;

        constexpr Vec2(const float x, const float y) : x(x), y(y) {
        }

        constexpr Vec2 operator+(const Vec2& rhs) const {
            return {x + rhs.x, y + rhs.y};
        }

        constexpr Vec2 operator-(const Vec2& rhs) const {
            return {x - rhs.x, y - rhs.y};
        }

        constexpr Vec2 operator*(const float scalar) const {
            return {x * scalar, y * scalar};
        }

        constexpr Vec2 operator/(const float scalar) const {
            if (scalar == 0) {
                throw std::runtime_error("Cannot divide by zero");
            }
            return {x / scalar, y / scalar};
        }

        constexpr Vec2 &operator+=(const Vec2& rhs) {
            x += rhs.x;
            y += rhs.y;
            return *this;
        }

        constexpr Vec2 &operator-=(const Vec2& rhs) {
            x -= rhs.x;
            y -= rhs.y;
            return *this;
        }

        constexpr Vec2 &operator*=(const float scalar) {
            x *= scalar;
            y *= scalar;
            return *this;
        }

        constexpr Vec2 &operator/=(const float scalar) {
            if (scalar == 0) {
                throw std::runtime_error("Cannot divide by zero");
            }
            x /= scalar;
            y /= scalar;
            return *this;
        }

        constexpr Vec2 operator-() const {
            return {-x, -y};
        }

        constexpr bool operator==(const Vec2& other) const {
            return x == other.x && y == other.y;
        }
    };

    constexpr Vec2 operator*(const float scalar, const Vec2& vec) {
        return vec * scalar;
    }
}
