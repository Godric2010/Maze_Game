#include "PlayerControllerSystem.hpp"

#include <iostream>
#include <ostream>


#include "GameWorld.hpp"
#include "InputReceiver.hpp"
#include "MotionIntent.hpp"
#include "components/Camera.hpp"
#include "components/Transform.hpp"

namespace Gameplay::Systems {
    PlayerControllerSystem::PlayerControllerSystem() = default;

    PlayerControllerSystem::~PlayerControllerSystem() = default;

    void PlayerControllerSystem::Initialize() {
    }

    void PlayerControllerSystem::Run(const float delta_time) {
        const auto player_entity = GameWorld()->GetEntityByName("Player");
        const auto input_receiver = GameWorld()->GetComponent<Engine::Core::Components::InputReceiver>(player_entity);
        if (input_receiver == nullptr) {
            return;
        }

        if (input_receiver->Input->IsKeyDown(Engine::Environment::Key::Esc)) {
            std::cout << "ESC" << std::endl;
            const auto pause_command = Commands::PauseCommand(true);
            GameWorld()->PushCommand(pause_command);
        }


        CalculateNewTransform(player_entity, input_receiver->Input);
    }

    void PlayerControllerSystem::CalculateNewTransform(const Engine::Ecs::EntityId player_entity,
                                                       const Engine::Environment::InputSnapshot *input) const {
        const auto transform = GameWorld()->GetComponent<Engine::Core::Components::Transform>(player_entity);
        const auto motion_intent = GameWorld()->GetComponent<Engine::Core::Components::MotionIntent>(player_entity);

        // Calculate camera rotation
        const auto mouse_delta = input->GetMouseDelta();
        const float yaw_delta = -mouse_delta.x * m_sensitivity;
        const float pitch_delta = -mouse_delta.y * m_sensitivity;

        const auto cam_rotation = transform->GetRotation();
        const float pitch = glm::clamp(cam_rotation.x + pitch_delta, m_min_pitch, m_max_pitch);
        const float yaw = cam_rotation.y + yaw_delta;
        const auto new_camera_rotation = glm::vec3(pitch, yaw, cam_rotation.z);
        motion_intent->rotation = new_camera_rotation;

        // Calculate camera position
        const auto forward_vec = normalize(glm::vec3(sin(glm::radians(yaw)), 0.0f, cos(glm::radians(yaw))));
        const auto right_vec = normalize(cross(forward_vec, glm::vec3(0, 1, 0)));

        auto camera_displacement = glm::vec3(0.0f);
        if (input->IsKeyHeld(Engine::Environment::Key::W)) {
            camera_displacement.z -= 1;
        }
        if (input->IsKeyHeld(Engine::Environment::Key::S)) {
            camera_displacement.z += 1;
        }
        if (input->IsKeyHeld(Engine::Environment::Key::A)) {
            camera_displacement.x += 1;
        }
        if (input->IsKeyHeld(Engine::Environment::Key::D)) {
            camera_displacement.x -= 1;
        }
        const glm::vec3 pos_delta = right_vec * camera_displacement.x + forward_vec * camera_displacement.z;

        motion_intent->translation = pos_delta;
        motion_intent->speed_multiplier = m_movement_speed;
    }
} // namespace
