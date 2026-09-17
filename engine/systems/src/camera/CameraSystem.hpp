#pragma once
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
        static math::Mat4 CalculatedViewMat(
                const components::TransformComponent* transform);
        static math::Mat4 CalculateProjectionMat(const components::Camera* camera_component);
    };
} // namespace
