#pragma once
#include "ISystem.hpp"

namespace Gameplay::Systems {
    ECS_SYSTEM(PauseSystem, Update, [])

    class PauseSystem : public Engine::Ecs::ISystem {
    public:
        PauseSystem();

        ~PauseSystem() override;

        void Initialize() override;

        void Run(float delta_time) override;
    };
} // namespace
