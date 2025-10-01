#pragma once
#include "Input.hpp"
#include "ISystem.hpp"

ECS_SYSTEM(InputReceiver, Input, [])
namespace Engine::Core::Systems {
    class InputReceiver : public Ecs::ISystem {
    public:
        InputReceiver();

        ~InputReceiver() override;

        void Initialize(Ecs::World *world, Ecs::IServiceToEcsProvider *service_locator) override;

        void Run(Ecs::World &world, float delta_time) override;

    private:
       Environment::IInput* m_input = nullptr;

    };
} // namespace