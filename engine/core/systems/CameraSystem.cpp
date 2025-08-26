#include "CameraSystem.hpp"

#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>

#include "components/Camera.hpp"
#include "components/Transform.hpp"

namespace Engine::Core::Systems {
    CameraSystem::CameraSystem() {
        m_front = glm::vec3(0.0f, 0.0f, -1.0f);
        m_up = glm::vec3(0.0f, 1.0f, 0.0f);
        m_right = glm::vec3(1.0f, 0.0f, 0.0f);
    }

    CameraSystem::~CameraSystem() = default;

    void CameraSystem::SetServices(Ecs::IServiceToEcsProvider *serviceLocator) {

    }

    void CameraSystem::Run(Ecs::World &world, float deltaTime) {
        for (const auto cameras = world.GetComponentsOfType<Components::Camera>(); auto camera: cameras) {
            const auto entity = camera.second;
            const auto cameraComponent = camera.first;
            const auto cameraTransform = world.GetComponent<Components::Transform>(entity);
            CalculateOrientation(cameraComponent, cameraTransform);
        }
    }

    void CameraSystem::CalculateOrientation(Components::Camera *cameraComponent, const Components::Transform *transform) {
        cameraComponent->projection = glm::perspective(glm::radians(cameraComponent->FieldOfView),
                                                       cameraComponent->AspectRatio,
                                                       cameraComponent->NearClip,
                                                       cameraComponent->FarClip);
        const float yawRad = glm::radians(transform->Rotation.y);
        const float pitchRad = glm::radians(transform->Rotation.x);

        glm::vec3 front;
        front.x = cos(yawRad) * cos(pitchRad);
        front.y = sin(pitchRad);
        front.z = sin(yawRad) * cos(pitchRad);
        m_front = glm::normalize(front);

        m_right = glm::normalize(glm::cross(m_front, WORLD_UP));
        m_up = glm::normalize(glm::cross(m_right, m_front));

        cameraComponent->view = glm::lookAt(m_front, transform->Position + m_right, m_up);
        cameraComponent->model = glm::mat4(1.0f);
    }
} // namespace
