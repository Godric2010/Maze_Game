#include "CameraControllerSystem.hpp"

#include <iostream>
#include <ostream>


#include "InputReceiver.hpp"
#include "MotionIntent.hpp"
#include "components/Camera.hpp"
#include "components/Transform.hpp"

namespace Gameplay {
    CameraControllerSystem::CameraControllerSystem() = default;

    CameraControllerSystem::~CameraControllerSystem() = default;

    void CameraControllerSystem::Initialize(Engine::Ecs::World *world, Engine::Ecs::IServiceToEcsProvider *service_locator) {
    }

    void CameraControllerSystem::Run(Engine::Ecs::World &world, const float delta_time) {
        for (const auto cameras = world.GetComponentsOfType<Engine::Core::Components::Camera>(); auto camera: cameras) {
            const auto entity = camera.second;
            const auto camera_transform = world.GetComponent<Engine::Core::Components::Transform>(entity);
            const auto camera_input = world.GetComponent<Engine::Core::Components::InputReceiver>(entity);
            const auto camera_motion_intent = world.GetComponent<Engine::Core::Components::MotionIntent>(entity);
            CalculateNewTransform(*camera_transform, *camera_motion_intent, camera_input->Input, delta_time);
        }
    }

    void CameraControllerSystem::CalculateNewTransform(Engine::Core::Components::Transform &transform,
                                                       Engine::Core::Components::MotionIntent &motion_intent,
                                                       const Engine::Environment::InputSnapshot *input,
                                                       const float delta_time) const {
        // Calculate camera rotation
        const auto mouse_delta = input->GetMouseDelta();
        const float yaw_delta = -mouse_delta.x * m_sensitivity;
        const float pitch_delta = -mouse_delta.y * m_sensitivity;

        const auto cam_rotation = transform.GetRotation();
        const float pitch = glm::clamp(cam_rotation.x + pitch_delta, m_min_pitch, m_max_pitch);
        const float yaw = cam_rotation.y + yaw_delta;
        const auto new_camera_rotation = glm::vec3(pitch, yaw, cam_rotation.z);
        motion_intent.rotation = new_camera_rotation;

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
            camera_displacement.x += m_velocity * delta_time;
        }
        if (input->IsKeyHeld(Engine::Environment::Key::D)) {
            camera_displacement.x -= m_velocity * delta_time;
        }
        const glm::vec3 pos_delta = right_vec * camera_displacement.x + forward_vec * camera_displacement.z;
        const auto camera_position = transform.GetPosition();
        auto new_camera_position = glm::vec3(camera_position.x + pos_delta.x, camera_position.y + pos_delta.y,
                                             camera_position.z + pos_delta.z);

        motion_intent.translation = new_camera_position;
    }
} // namespace
