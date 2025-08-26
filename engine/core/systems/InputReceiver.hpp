#pragma once
#include "Input.hpp"
#include "ISystem.hpp"

ECS_SYSTEM(InputReceiver, Input, [])
namespace Engine::Core::Systems {
    class InputReceiver : public Ecs::ISystem {
    public:
        InputReceiver();

        ~InputReceiver() override;

        void SetServices(Ecs::IServiceToEcsProvider *serviceLocator) override;

        void Run(Ecs::World &world, float deltaTime) override;

    private:
       Environment::IInput* m_input = nullptr;

    };
} // namespace