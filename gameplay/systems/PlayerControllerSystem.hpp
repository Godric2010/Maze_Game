#pragma once
#include "Input/InputBuffer.hpp"
#include "IEngineSystem.hpp"

ECS_SYSTEM(PlayerControllerSystem, Input, TAGS(), DEPENDENCIES())

namespace gameplay::systems {
    class PlayerControllerSystem : public yarep::ecs::ISystem {
    public:
        PlayerControllerSystem();

        ~PlayerControllerSystem() override;

        void Initialize() override;

        void Run(float delta_time) override;

    private:
        const float m_movement_speed = 1.0f;
        const float m_sensitivity = 0.6f;

        mutable yarep::math::Angle m_yaw_target{};
        mutable yarep::math::Angle m_pitch_target{};
        mutable bool m_initialized = false;

        const yarep::math::Angle m_min_pitch = yarep::math::Angle::from_degrees(-60.0f);
        const yarep::math::Angle m_max_pitch = yarep::math::Angle::from_degrees(75.0f);

        void CalculateNewTransform(
                yarep::ecs::EntityId player_entity, const yarep::input::InputBuffer& input, float delta_time) const;
    };
} // namespace
