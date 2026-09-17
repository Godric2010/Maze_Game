#pragma once
#include "IEngineSystem.hpp"

ECS_SYSTEM(EngineEventSystem, EngineEvents, TAGS(ENGINE), DEPENDENCIES())

namespace yarep::systems {
    struct TransformCommand {
        math::Vec3 translation;
        math::Vec3 rotation;
        math::Vec3 scale;
    };

    class EngineEventSystem final : public ecs::IEngineSystem {
    public:
        EngineEventSystem() = default;

        ~EngineEventSystem() override = default;

        void Initialize() override;

        void Run(float delta_time) override;
    };
} // namespace
