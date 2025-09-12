#include "CameraControllerSystem.hpp"

#include <iostream>
#include <ostream>


#include "InputReceiver.hpp"
#include "components/Camera.hpp"
#include "components/Transform.hpp"

namespace Gameplay {
    CameraControllerSystem::CameraControllerSystem() = default;

    CameraControllerSystem::~CameraControllerSystem() = default;

    void CameraControllerSystem::SetServices(Engine::Ecs::IServiceToEcsProvider *serviceLocator) {
    }

    void CameraControllerSystem::Run(Engine::Ecs::World &world, const float deltaTime) {
        for (const auto cameras = world.GetComponentsOfType<Engine::Core::Components::Camera>(); auto camera: cameras) {
            const auto entity = camera.second;
            const auto camera_transform = world.GetComponent<Engine::Core::Components::Transform>(entity);
            const auto camera_input = world.GetComponent<Engine::Core::Components::InputReceiver>(entity);
            CalculateNewTransform(*camera_transform, camera_input->Input, deltaTime);
        }
    }

    void CameraControllerSystem::CalculateNewTransform(Engine::Core::Components::Transform &transform,
                                                       const Engine::Environment::InputSnapshot *input,
                                                       const float delta_time) const {
        // Calculate camera rotation
        const auto mouse_delta = input->GetMouseDelta();
        const float yaw_delta = -mouse_delta.x * m_sensitivity;
        const float pitch_delta = -mouse_delta.y * m_sensitivity;

        const float pitch = glm::clamp(transform.rotation.x + pitch_delta, m_min_pitch, m_max_pitch);
        const float yaw = transform.rotation.y + yaw_delta;
        transform.rotation = glm::vec3(pitch, yaw, transform.rotation.z);

        // Calculate camera position
        const auto forward_vec = normalize(glm::vec3(sin(glm::radians(yaw)), 0.0f, cos(glm::radians(yaw))));
        const auto right_vec = normalize(cross(forward_vec, glm::vec3(0, 1, 0)));

        auto camera_displacement = glm::vec3(0.0f);
        if (input->IsKeyHeld(Engine::Environment::Key::W)) {
            camera_displacement.z -= m_velocity * delta_time;
        }
        if (input->IsKeyHeld(Engine::Environment::Key::S)) {
            camera_displacement.z += m_velocity * delta_time;
        }
        if (input->IsKeyHeld(Engine::Environment::Key::A)) {
            camera_displacement.x -= m_velocity * delta_time;
        }
        if (input->IsKeyHeld(Engine::Environment::Key::D)) {
            camera_displacement.x += m_velocity * delta_time;
        }
        const glm::vec3 pos_delta= right_vec * camera_displacement.x + forward_vec * camera_displacement.z;
        transform.position += pos_delta;
    }
} // namespace
