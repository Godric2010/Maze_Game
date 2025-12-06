#include "../include/CameraSystem.hpp"

namespace Engine::Core::Systems {
    CameraSystem::CameraSystem() = default;

    CameraSystem::~CameraSystem() = default;

    void CameraSystem::Initialize() {
    }

    void CameraSystem::Run(float delta_time) {
        for (const auto cameras = EcsWorld()->GetComponentsOfType<Components::Camera>(); auto camera: cameras) {
            const auto entity = camera.second;
            const auto camera_component = camera.first;
            const auto camera_transform = EcsWorld()->GetComponent<Components::Transform>(entity);
            CalculateOrientation(camera_component, camera_transform);
        }
    }

    void CameraSystem::CalculateOrientation(Components::Camera *camera_component,
                                            const Components::Transform *transform) {
        camera_component->projection = glm::perspective(glm::radians(camera_component->FieldOfView),
                                                        camera_component->AspectRatio,
                                                        camera_component->NearClip,
                                                        camera_component->FarClip);

        const auto cam_rotation = transform->GetRotation();
        const float pitch_rad = glm::radians(cam_rotation.x);
        const float yaw_rad = glm::radians(cam_rotation.y);
        const float roll_rad = glm::radians(cam_rotation.z);

        const glm::mat4 r = glm::yawPitchRoll(yaw_rad, pitch_rad, roll_rad);

        constexpr auto local_forward = glm::vec3(0, 0, -1);
        constexpr auto local_up = glm::vec3(0, 1, 0);
        const glm::vec3 forward = glm::normalize(r * glm::vec4(local_forward, 0.0f));
        const glm::vec3 up = glm::normalize(r * glm::vec4(local_up, 0.0f));

        const glm::vec3 eye = transform->GetPosition();
        const glm::vec3 target = eye + forward;

        camera_component->view = glm::lookAt(eye, target, up);
        camera_component->model = glm::mat4(1.0f);
    }
} // namespace
