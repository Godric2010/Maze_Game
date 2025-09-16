#pragma once
#include <glm/glm.hpp>
#include <glm/fwd.hpp>

#include "ISystem.hpp"

ECS_SYSTEM(TransformSystem, LateUpdate, [])
namespace Engine::Core::Systems {
    class TransformSystem : public Ecs::ISystem {
    public:
        TransformSystem();

        ~TransformSystem() override;

        void SetServices(Ecs::IServiceToEcsProvider *service_locator) override;

        void Run(Ecs::World &world, float delta_time) override;

    private:
        static glm::mat4 CalculateMatrix(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale);
    };
} // namespace
