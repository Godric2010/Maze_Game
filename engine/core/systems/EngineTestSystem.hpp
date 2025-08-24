#pragma once
#include "ISystem.hpp"

namespace Engine::Core::Systems {
    ECS_SYSTEM(EngineTestSystem, Update, [core])

    class EngineTestSystem : public Ecs::ISystem {
    public:
        EngineTestSystem();

        ~EngineTestSystem() override;

        void Run(Ecs::World &world, float deltaTime) override;

    private:
    };
} // namespace
