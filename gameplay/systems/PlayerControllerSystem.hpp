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

        mutable float m_yaw_target = 0.0f;
        mutable float m_pitch_target = 0.0f;
        mutable bool m_initialized = false;

        const float m_min_pitch = -60.0f;
        const float m_max_pitch = 75.0f;

        void CalculateNewTransform(
                yarep::ecs::EntityId player_entity, const yarep::input::InputBuffer& input, float delta_time) const;
    };
} // namespace
