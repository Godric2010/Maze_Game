#pragma once
#include "Quaternion.hpp"
#include "Vec3.hpp"

namespace yarep::math {
    struct Transform {
        Vec3 position{};
        Quaternion rotation{};
        Vec3 scale{1.0f, 1.0f, 1.0f};

        constexpr Transform() = default;

        constexpr Transform(const Vec3& position, const Quaternion& rotation, const Vec3& scale) : position{position},
            rotation{rotation},
            scale{scale} {
        }

        static constexpr Transform identity() {
            return {};
        }

        constexpr bool operator==(const Transform&) const = default;
    };
}
