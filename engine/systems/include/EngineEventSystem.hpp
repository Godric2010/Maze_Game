#pragma once
#include <queue>
#include <glm/vec3.hpp>

#include "ISystem.hpp"

ECS_SYSTEM(EngineEventSystem, EngineEvents, [ENGINE])

namespace Engine::Core::Systems {
    struct TransformCommand {
        glm::vec3 translation;
        glm::vec3 rotation;
        glm::vec3 scale;
    };

    class EngineEventSystem final : public Ecs::IEngineSystem {
    public:
        EngineEventSystem() = default;

        ~EngineEventSystem() override = default;

        void Initialize() override;

        void Run(float delta_time) override;
    };
} // namespace
