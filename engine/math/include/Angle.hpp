#pragma once
#include <numbers>
#include <stdexcept>

namespace yarep::math {
    class Angle {
    public:
        constexpr Angle() = default;

        static constexpr Angle from_radians(const float radians) {
            return Angle{radians};
        }

        static constexpr Angle from_degrees(const float degrees) {
            return Angle{degrees * std::numbers::pi_v<float> / 180.0f};
        }

        [[nodiscard]] constexpr float radians() const {
            return radians_;
        }

        [[nodiscard]] constexpr float degrees() const {
            return radians_ * 180.0f / std::numbers::pi_v<float>;
        }

        constexpr Angle operator+(const Angle rhs) const {
            return Angle{radians_ + rhs.radians_};
        }

        constexpr Angle operator-(const Angle rhs) const {
            return Angle{radians_ - rhs.radians_};
        }

        constexpr Angle operator*(const float scalar) const {
            return Angle{radians_ * scalar};
        }

        constexpr Angle operator/(const float scalar) const {
            if (scalar == 0.0f) {
                throw std::runtime_error("Cannot divide angle by zero");
            }
            return Angle{radians_ / scalar};
        }

        constexpr bool operator==(const Angle rhs) const {
            return radians_ == rhs.radians_;
        }

    private:
        constexpr explicit Angle(const float radians) : radians_(radians) {
        }

        float radians_ = 0.0f;
    };

    constexpr Angle operator*(const float scalar, const Angle angle) {
        return angle * scalar;
    }
}
