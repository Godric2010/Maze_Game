#pragma once
#include <glm/vec3.hpp>
#include <glm/gtc/matrix_transform.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/euler_angles.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>

#include "components/Camera.hpp"
#include "components/Transform.hpp"
#include "ISystem.hpp"

ECS_SYSTEM(CameraSystem, LateUpdate, [])

namespace Engine::Core::Systems {
    class CameraSystem final : public Ecs::ISystem {
    public:
        CameraSystem();

        ~CameraSystem() override;

        void Initialize(Ecs::World *world, Ecs::IServiceToEcsProvider *service_locator) override;

        void Run(Ecs::World &world, float delta_time) override;

    private:
        static void CalculateOrientation(Components::Camera *camera_component, const Components::Transform *transform);
    };
} // namespace
