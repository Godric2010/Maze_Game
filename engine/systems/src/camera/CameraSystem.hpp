#pragma once
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/euler_angles.hpp>

#include <Camera.hpp>
#include <TransformComponent.hpp>
#include "IEngineSystem.hpp"

ECS_SYSTEM(CameraSystem, LateUpdate, TAGS(ENGINE), DEPENDENCIES())

namespace yarep::systems {
    class CameraSystem final : public ecs::IEngineSystem {
    public:
        CameraSystem();

        ~CameraSystem() override;

        void Initialize() override;

        void Run(float delta_time) override;

    private:
        static glm::mat4 CalculatedViewMat(
                const components::TransformComponent* transform);
        static glm::mat4 CalculateProjectionMat(const components::Camera *camera_component);
    };
} // namespace
