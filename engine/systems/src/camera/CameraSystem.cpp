#include "CameraSystem.hpp"

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

    math::Mat4 CameraSystem::CalculatedViewMat(const components::TransformComponent* transform) {
        const auto cam_rotation = transform->GetRotation();

        constexpr auto local_forward = math::Vec3(0, 0, -1);
        constexpr auto local_up = math::Vec3(0, 1, 0);
        const auto forward = math::normalize(math::rotate(cam_rotation, local_forward));
        const auto up = math::normalize(math::rotate(cam_rotation, local_up));

        const math::Vec3 eye = transform->GetPosition();
        const math::Vec3 target = eye + forward;

        return math::look_at(eye, target, up);
    }

    math::Mat4 CameraSystem::CalculateProjectionMat(const components::Camera* camera_component) {
        const auto fov_y = math::Angle::from_degrees(camera_component->GetFieldOfView());
        return math::perspective(fov_y,
                                 camera_component->GetAspectRatio(),
                                 camera_component->GetNearClip(),
                                 camera_component->GetFarClip()
                );
    }
} // namespace
