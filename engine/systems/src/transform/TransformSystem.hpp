#pragma once
#include "IEngineSystem.hpp"
ECS_SYSTEM(TransformSystem, LateUpdate, TAGS(ENGINE), DEPENDENCIES())

namespace yarep::systems {
    class TransformSystem : public ecs::IEngineSystem {
    public:
        TransformSystem();

        ~TransformSystem() override;

        void Initialize() override;

        void Run(float delta_time) override;
    };
} // namespace
