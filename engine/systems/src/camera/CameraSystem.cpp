#include "CameraSystem.hpp"

#include <glm/gtx/quaternion.hpp>

namespace yarep::systems {
    CameraSystem::CameraSystem() = default;

    CameraSystem::~CameraSystem() = default;

    void CameraSystem::Initialize() {
        EcsWorld()->GetComponentEventBus()->SubscribeOnComponentAddEvent<components::Camera>(
                [this](const ecs::EntityId entity, const components::Camera& _) {
                    if (this->Cache()->GetCameraCache() == nullptr) {
                        throw std::runtime_error("CameraSystem::Initialize() - cache is null");
                    }
                    this->Cache()->GetCameraCache()->RegisterEntity(entity);
                }
                );
        EcsWorld()->GetComponentEventBus()->SubscribeOnComponentRemoveEvent<components::Camera>(
                [this](const ecs::EntityId entity) {
                    this->Cache()->GetCameraCache()->DeregisterEntity(entity);
                }
                );
    }

    void CameraSystem::Run(float delta_time) {
        auto camera_components = EcsWorld()->GetComponentsOfType<components::Camera>();
        for (const auto [camera, entity]: camera_components) {
            const auto camera_transform = EcsWorld()->GetComponent<components::TransformComponent>(entity);
            auto view_mat = CalculatedViewMat(camera_transform);

            const auto cache_val = Cache()->GetCameraCache()->GetCacheValue(entity);
            auto proj_mat = cache_val.projection;
            if (cache_val.version != camera->GetVersion()) {
                proj_mat = CalculateProjectionMat(camera);
            }

            Cache()->GetCameraCache()->SetCacheValue(entity, view_mat, proj_mat, camera->GetVersion());
        }
    }

    //Temp function
    static glm::quat to_glm_quat(const math::Quaternion& quaternion) {
        return glm::quat{
            quaternion.w,
            quaternion.x,
            quaternion.y,
            quaternion.z
        };
    }

    glm::mat4 CameraSystem::CalculatedViewMat(const components::TransformComponent* transform) {
        const auto cam_rotation = transform->GetRotation();

        const glm::mat4 r = glm::toMat4(to_glm_quat(cam_rotation));

        constexpr auto local_forward = glm::vec3(0, 0, -1);
        constexpr auto local_up = glm::vec3(0, 1, 0);
        const glm::vec3 forward = normalize(r * glm::vec4(local_forward, 0.0f));
        const glm::vec3 up = normalize(r * glm::vec4(local_up, 0.0f));

        const glm::vec3 eye = glm::vec3(transform->GetPosition().x,
                                        transform->GetPosition().y,
                                        transform->GetPosition().z
                );
        const glm::vec3 target = eye + forward;

        return lookAt(eye, target, up);
    }

    glm::mat4 CameraSystem::CalculateProjectionMat(const components::Camera* camera_component) {
        return glm::perspective(glm::radians(camera_component->GetFieldOfView()),
                                camera_component->GetAspectRatio(),
                                camera_component->GetNearClip(),
                                camera_component->GetFarClip()
                );
    }
} // namespace
