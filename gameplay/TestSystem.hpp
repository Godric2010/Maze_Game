#pragma once
#include "ISystem.hpp"

namespace Gameplay {

    ECS_SYSTEM(TestSystem, Update, [core])

    class TestSystem final : public Engine::Ecs::ISystem {
    public:
        TestSystem();

        ~TestSystem() override;

        void Run(Engine::Ecs::World &world, float deltaTime) override;

    private:
    };
} // namespace