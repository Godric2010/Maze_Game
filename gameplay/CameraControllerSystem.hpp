#pragma once
#include "InputReceiver.hpp"
#include "ISystem.hpp"
#include "MotionIntent.hpp"
#include "components/Transform.hpp"

ECS_SYSTEM(CameraControllerSystem, Input, [])

namespace Gameplay {
    class CameraControllerSystem : public Engine::Ecs::ISystem {
    public:
        CameraControllerSystem();

        ~CameraControllerSystem() override;

        void Initialize(Engine::Ecs::World *world, Engine::Ecs::IServiceToEcsProvider *service_locator) override;

        void Run(Engine::Ecs::World &world, float delta_time) override;

    private:
        const float m_movement_speed = 0.6f;
        const float m_sensitivity = 0.4f;

        const float m_min_pitch = -60.0f;
        const float m_max_pitch = 75.0f;

        void CalculateNewTransform(Engine::Core::Components::Transform& transform,
                                   Engine::Core::Components::MotionIntent &motion_intent,
                                   const Engine::Environment::InputSnapshot *input, float delta_time) const;
    };
} // namespace
