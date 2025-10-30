#pragma once
#include "Input.hpp"
#include "ISystem.hpp"
#include "MotionIntent.hpp"
#include "components/Transform.hpp"
#include "../commands/PauseCommand.hpp"

ECS_SYSTEM(PlayerControllerSystem, Input, [])

namespace Gameplay::Systems {
    class PlayerControllerSystem : public Engine::Ecs::ISystem {
    public:
        PlayerControllerSystem();

        ~PlayerControllerSystem() override;

        void Initialize() override;

        void Run(float delta_time) override;

    private:
        const float m_movement_speed = 0.6f;
        const float m_sensitivity = 0.4f;

        const float m_min_pitch = -60.0f;
        const float m_max_pitch = 75.0f;

        void CalculateNewTransform(
            Engine::Ecs::EntityId player_entity, const Engine::Environment::InputSnapshot *input) const;
    };
} // namespace
