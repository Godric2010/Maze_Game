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
#include "components/Camera.hpp"
#include "components/Transform.hpp"

ECS_SYSTEM(CameraSystem, LateUpdate, [])

namespace Engine::Core::Systems {
    class CameraSystem : public Ecs::ISystem {
    public:
        CameraSystem();

        ~CameraSystem() override;

        void SetServices(Ecs::IServiceToEcsProvider *serviceLocator) override;

        void Run(Ecs::World &world, float deltaTime) override;

    private:
        void CalculateOrientation(Components::Camera *cameraComponent, const Components::Transform *transform);
    };
} // namespace
