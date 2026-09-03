#pragma once

namespace yarep::math {
    struct Quaternion {
        float x = 0.0f;
        float y = 0.0f;
        float z = 0.0f;
        float w = 1.0f;

        constexpr Quaternion() = default;

        constexpr Quaternion(const float x, const float y, const float z, const float w) : x(x), y(y), z(z), w(w) {
        }

        static constexpr Quaternion identity() {
            return {};
        }

        constexpr Quaternion operator*(const Quaternion& rhs) const {
            return {
                w * rhs.x + x * rhs.w + y * rhs.z - z * rhs.y,
                w * rhs.y - x * rhs.z + y * rhs.w + z * rhs.x,
                w * rhs.z + x * rhs.y - y * rhs.x + z * rhs.w,
                w * rhs.w - x * rhs.x - y * rhs.y - z * rhs.z
            };
        }

        constexpr Quaternion operator*=(const Quaternion& rhs) {
            *this = *this * rhs;
            return *this;
        }

        constexpr bool operator==(const Quaternion&) const = default;
    };
}
