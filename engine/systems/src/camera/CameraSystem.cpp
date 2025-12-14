#include "CameraSystem.hpp"

namespace Engine::Systems {
    CameraSystem::CameraSystem() = default;

    CameraSystem::~CameraSystem() = default;

    void CameraSystem::Initialize() {
        EcsWorld()->GetComponentEventBus()->SubscribeOnComponentAddEvent<Components::Camera>(
                [this](const Ecs::EntityId entity, const Components::Camera& _) {
                    if (this->Cache()->GetCameraCache() == nullptr) {
                        throw std::runtime_error("CameraSystem::Initialize() - cache is null");
                    }
                    this->Cache()->GetCameraCache()->RegisterEntity(entity);
                }
                );
        EcsWorld()->GetComponentEventBus()->SubscribeOnComponentRemoveEvent<Components::Camera>(
                [this](const Ecs::EntityId entity) {
                    this->Cache()->GetCameraCache()->DeregisterEntity(entity);
                }
                );
    }

    void CameraSystem::Run(float delta_time) {
        auto camera_components = EcsWorld()->GetComponentsOfType<Components::Camera>();
        for (const auto [camera, entity]: camera_components) {
            const auto camera_transform = EcsWorld()->GetComponent<Components::Transform>(entity);
            auto view_mat = CalculatedViewMat(camera_transform);

            const auto cache_val = Cache()->GetCameraCache()->GetCacheValue(entity);
            auto proj_mat = cache_val.projection;
            if (cache_val.version != camera->GetVersion()) {
                proj_mat = CalculateProjectionMat(camera);
            }

            Cache()->GetCameraCache()->SetCacheValue(entity, view_mat, proj_mat, camera->GetVersion());
        }
    }

    glm::mat4 CameraSystem::CalculatedViewMat(const Components::Transform* transform) {
        const auto cam_rotation = transform->GetRotation();
        const float pitch_rad = glm::radians(cam_rotation.x);
        const float yaw_rad = glm::radians(cam_rotation.y);
        const float roll_rad = glm::radians(cam_rotation.z);

        const glm::mat4 r = glm::yawPitchRoll(yaw_rad, pitch_rad, roll_rad);

        constexpr auto local_forward = glm::vec3(0, 0, -1);
        constexpr auto local_up = glm::vec3(0, 1, 0);
        const glm::vec3 forward = normalize(r * glm::vec4(local_forward, 0.0f));
        const glm::vec3 up = normalize(r * glm::vec4(local_up, 0.0f));

        const glm::vec3 eye = transform->GetPosition();
        const glm::vec3 target = eye + forward;

        return lookAt(eye, target, up);
    }

    glm::mat4 CameraSystem::CalculateProjectionMat(const Components::Camera* camera_component) {
        return glm::perspective(glm::radians(camera_component->GetFieldOfView()),
                                camera_component->GetAspectRatio(),
                                camera_component->GetNearClip(),
                                camera_component->GetFarClip()
                );
    }
} // namespace
