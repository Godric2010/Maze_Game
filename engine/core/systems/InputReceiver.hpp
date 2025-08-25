#pragma once
#include "ISystem.hpp"

ECS_SYSTEM(InputReceiver, Input, [])
namespace Engine::Core::Systems {
    class InputReceiver : public Ecs::ISystem {
    public:
        InputReceiver();

        ~InputReceiver() override;

        void Run(Ecs::World &world, float deltaTime) override;

    private:

    };
} // namespace