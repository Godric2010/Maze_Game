#pragma once
#include <queue>
#include <glm/vec3.hpp>

#include "ISystem.hpp"

ECS_SYSTEM(CommandSystem, Commands, [ENGINE])

namespace Engine::Core::Systems {
    struct TransformCommand {
        glm::vec3 translation;
        glm::vec3 rotation;
        glm::vec3 scale;
    };

    class CommandSystem final : public Ecs::IEngineSystem {
    public:
        CommandSystem() = default;

        ~CommandSystem() override = default;

        void Initialize() override;

        void Run(float delta_time) override;
    };
} // namespace
