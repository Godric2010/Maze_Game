#pragma once
#include "IEngineSystem.hpp"

namespace gameplay::systems {
    ECS_SYSTEM(PauseSystem, Update, TAGS(), DEPENDENCIES())

    class PauseSystem : public yarep::Ecs::ISystem {
    public:
        PauseSystem();

        ~PauseSystem() override;

        void Initialize() override;

        void Run(float delta_time) override;
    };
} // namespace
