#pragma once
#include "ISystem.hpp"

ECS_SYSTEM(EngineTestSystem, Update, [core])
namespace Engine::Core::Systems {
    class EngineTestSystem : public Ecs::ISystem {
    public:
        EngineTestSystem();

        ~EngineTestSystem() override;

        void Run(Ecs::World &world, float deltaTime) override;

    private:
        std::string test;
    };
} // namespace
