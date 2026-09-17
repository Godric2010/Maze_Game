#pragma once
#include "Angle.hpp"

namespace gameplay::components {
    struct FirstPersonControllerComponent {
    private:
        yarep::math::Angle yaw{};
        yarep::math::Angle pitch{};

        yarep::math::Angle target_yaw{};
        yarep::math::Angle target_pitch{};

    public:
        FirstPersonControllerComponent() = default;

        FirstPersonControllerComponent(const yarep::math::Angle& yaw,
                                       const yarep::math::Angle& pitch) : yaw(yaw), pitch(pitch) {
            target_yaw = yaw;
            target_pitch = pitch;
        };

        FirstPersonControllerComponent &SetYaw(const yarep::math::Angle& yaw) {
            this->yaw = yaw;
            return *this;
        }

        FirstPersonControllerComponent &SetPitch(const yarep::math::Angle& pitch) {
            this->pitch = pitch;
            return *this;
        }

        FirstPersonControllerComponent &SetTargetYaw(const yarep::math::Angle& yaw) {
            this->target_yaw = yaw;
            return *this;
        }

        FirstPersonControllerComponent &SetTargetPitch(const yarep::math::Angle& pitch) {
            this->target_pitch = pitch;
            return *this;
        }

        [[nodiscard]] yarep::math::Angle GetYaw() const {
            return this->yaw;
        }

        [[nodiscard]] yarep::math::Angle GetPitch() const {
            return this->pitch;
        }

        [[nodiscard]] yarep::math::Angle GetTargetYaw() const {
            return this->target_yaw;
        }

        [[nodiscard]] yarep::math::Angle GetTargetPitch() const {
            return this->target_pitch;
        }
    };
}
