#pragma once
#include <algorithm>
#include <cmath>
#include "Vec2.hpp"
#include "Vec3.hpp"
#include "Vec4.hpp"

namespace yarep::math {
    constexpr float length_squared(const Vec2& vec) {
        return vec.x * vec.x + vec.y * vec.y;
    }

    constexpr float length_squared(const Vec3& vec) {
        return vec.x * vec.x + vec.y * vec.y + vec.z * vec.z;
    }

    constexpr float length_squared(const Vec4& vec) {
        return vec.x * vec.x + vec.y * vec.y + vec.z * vec.z + vec.w * vec.w;
    }

    inline float length(const Vec2& vec) {
        return std::sqrt(length_squared(vec));
    }

    inline float length(const Vec3& vec) {
        return std::sqrt(length_squared(vec));
    }

    inline float length(const Vec4& vec) {
        return std::sqrt(length_squared(vec));
    }

    constexpr Vec2 normalize(const Vec2& vec) {
        const float len = length(vec);
        if (len == 0.0f) {
            return {};
        }
        return vec / len;
    }

    constexpr Vec3 normalize(const Vec3& vec) {
        const float len = length(vec);
        if (len == 0.0f) {
            return {};
        }
        return vec / len;
    }

    constexpr Vec4 normalize(const Vec4& vec) {
        const float len = length(vec);
        if (len == 0.0f) {
            return {};
        }
        return vec / len;
    }

    constexpr float dot(const Vec2& lhs, const Vec2& rhs) {
        return lhs.x * rhs.x + lhs.y * rhs.y;
    }

    constexpr float dot(const Vec3& lhs, const Vec3& rhs) {
        return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z;
    }

    constexpr float dot(const Vec4& lhs, const Vec4& rhs) {
        return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z + lhs.w * rhs.w;
    }

    constexpr Vec3 cross(const Vec3& lhs, const Vec3& rhs) {
        return {
            lhs.y * rhs.z - lhs.z * rhs.y,
            lhs.z * rhs.x - lhs.x * rhs.z,
            lhs.x * rhs.y - lhs.y * rhs.x
        };
    }

    constexpr float distance_squared(const Vec2& lhs, const Vec2& rhs) {
        return length_squared(rhs - lhs);
    }

    constexpr float distance_squared(const Vec3& lhs, const Vec3& rhs) {
        return length_squared(rhs - lhs);
    }

    constexpr float distance_squared(const Vec4& lhs, const Vec4& rhs) {
        return length_squared(rhs - lhs);
    }

    inline float distance(const Vec2& lhs, const Vec2& rhs) {
        return std::sqrt(distance_squared(lhs, rhs));
    }

    inline float distance(const Vec3& lhs, const Vec3& rhs) {
        return std::sqrt(distance_squared(lhs, rhs));
    }

    inline float distance(const Vec4& lhs, const Vec4& rhs) {
        return std::sqrt(distance_squared(lhs, rhs));
    }

    constexpr Vec2 lerp(const Vec2& lhs, const Vec2& rhs, const float t) {
        return lhs + t * (rhs - lhs);
    }

    constexpr Vec3 lerp(const Vec3& lhs, const Vec3& rhs, const float t) {
        return lhs + t * (rhs - lhs);
    }

    constexpr Vec4 lerp(const Vec4& lhs, const Vec4& rhs, const float t) {
        return lhs + t * (rhs - lhs);
    }

    constexpr Vec2 min(const Vec2& lhs, const Vec2& rhs) {
        return {std::min(lhs.x, rhs.x), std::min(lhs.y, rhs.y)};
    }

    constexpr Vec3 min(const Vec3& lhs, const Vec3& rhs) {
        return {std::min(lhs.x, rhs.x), std::min(lhs.y, rhs.y), std::min(lhs.z, rhs.z)};
    }

    constexpr Vec4 min(const Vec4& lhs, const Vec4& rhs) {
        return {std::min(lhs.x, rhs.x), std::min(lhs.y, rhs.y), std::min(lhs.z, rhs.z), std::min(lhs.w, rhs.w)};
    }

    constexpr Vec2 max(const Vec2& lhs, const Vec2& rhs) {
        return {std::max(lhs.x, rhs.x), std::max(lhs.y, rhs.y)};
    }

    constexpr Vec3 max(const Vec3& lhs, const Vec3& rhs) {
        return {std::max(lhs.x, rhs.x), std::max(lhs.y, rhs.y), std::max(lhs.z, rhs.z)};
    }

    constexpr Vec4 max(const Vec4& lhs, const Vec4& rhs) {
        return {std::max(lhs.x, rhs.x), std::max(lhs.y, rhs.y), std::max(lhs.z, rhs.z), std::max(lhs.w, rhs.w)};
    }

    constexpr Vec2 clamp(const Vec2& vec, const Vec2& minimum, const Vec2& maximum) {
        return {
            std::clamp(vec.x, minimum.x, maximum.x),
            std::clamp(vec.y, minimum.y, maximum.y)
        };
    }

    constexpr Vec3 clamp(const Vec3& vec, const Vec3& minimum, const Vec3& maximum) {
        return {
            std::clamp(vec.x, minimum.x, maximum.x),
            std::clamp(vec.y, minimum.y, maximum.y),
            std::clamp(vec.z, minimum.z, maximum.z)
        };
    }

    constexpr Vec4 clamp(const Vec4& vec, const Vec4& minimum, const Vec4& maximum) {
        return {
            std::clamp(vec.x, minimum.x, maximum.x),
            std::clamp(vec.y, minimum.y, maximum.y),
            std::clamp(vec.z, minimum.z, maximum.z),
            std::clamp(vec.w, minimum.w, maximum.w)
        };
    }

    constexpr bool nearly_equal(const Vec2& lhs, const Vec2& rhs, const float tolerance) {
        return std::abs(lhs.x - rhs.x) <= tolerance
               && std::abs(lhs.y - rhs.y) <= tolerance;
    }

    constexpr bool nearly_equal(const Vec3& lhs, const Vec3& rhs, const float tolerance) {
        return std::abs(lhs.x - rhs.x) <= tolerance &&
               std::abs(lhs.y - rhs.y) <= tolerance &&
               std::abs(lhs.z - rhs.z) <= tolerance;
    }

    constexpr bool nearly_equal(const Vec4& lhs, const Vec4& rhs, const float tolerance) {
        return std::abs(lhs.x - rhs.x) <= tolerance &&
               std::abs(lhs.y - rhs.y) <= tolerance &&
               std::abs(lhs.z - rhs.z) <= tolerance &&
               std::abs(lhs.w - rhs.w) <= tolerance;
    }
}
