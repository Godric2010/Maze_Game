#pragma once
#include <queue>
#include <glm/vec3.hpp>

#include "ISystem.hpp"

ECS_SYSTEM(CommandSystem, Commands, [])

namespace Engine::Core::Systems {
    struct TransformCommand {
        glm::vec3 translation;
        glm::vec3 rotation;
        glm::vec3 scale;
    };

    class CommandSystem final : public Ecs::ISystem {
    public:
        CommandSystem() = default;

        ~CommandSystem() override = default;

        void SetServices(Ecs::IServiceToEcsProvider *service_locator) override;

        void Run(Ecs::World &world, float delta_time) override;
    };
} // namespace
