#pragma once
#include "InputReceiver.hpp"
#include "ISystem.hpp"
#include "components/Transform.hpp"

ECS_SYSTEM(CameraControllerSystem, Update, [])

namespace Gameplay {
    class CameraControllerSystem : public Engine::Ecs::ISystem {
    public:
        CameraControllerSystem();

        ~CameraControllerSystem() override;

        void SetServices(Engine::Ecs::IServiceToEcsProvider *serviceLocator) override;

        void Run(Engine::Ecs::World &world, float deltaTime) override;

    private:
        const float m_velocity = 1.0f;
        const float m_sensitivity = 0.4f;

        const float m_min_pitch = -60.0f;
        const float m_max_pitch = 75.0f;

        void CalculateNewTransform(Engine::Core::Components::Transform& transform,
                                   const Engine::Environment::InputSnapshot *input,
                                   float delta_time) const;
    };
} // namespace
