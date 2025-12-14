#pragma once
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/euler_angles.hpp>

#include <Camera.hpp>
#include <Transform.hpp>
#include "ISystem.hpp"

ECS_SYSTEM(CameraSystem, LateUpdate, [ENGINE])

namespace Engine::Systems {
    class CameraSystem final : public Ecs::IEngineSystem {
    public:
        CameraSystem();

        ~CameraSystem() override;

        void Initialize() override;

        void Run(float delta_time) override;

    private:
        static glm::mat4 CalculatedViewMat(
                const Components::Transform* transform);
        static glm::mat4 CalculateProjectionMat(const Components::Camera *camera_component);
    };
} // namespace
