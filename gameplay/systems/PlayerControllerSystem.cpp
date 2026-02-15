#include "PlayerControllerSystem.hpp"

#include <iostream>
#include <ostream>

#include "Rigidbody.hpp"
#include "SystemWorld.hpp"
#include "Transform.hpp"
#include "../commands/PauseCommand.hpp"

namespace Gameplay::Systems
{
    PlayerControllerSystem::PlayerControllerSystem() = default;

    PlayerControllerSystem::~PlayerControllerSystem() = default;

    void PlayerControllerSystem::Initialize()
    {
    }

    void PlayerControllerSystem::Run(const float delta_time)
    {
        const auto input = Input()->GetInput();
        if (!input.IsMapActive("PlayerInputMap"))
            return;

        const auto player_entity = GameWorld()->GetEntityByName("Player");

        if (input.HasAction("pause"))
        {
            std::cout << "Enabled pause!" << std::endl;

            const auto pause_command = Commands::PauseCommand(true);
            SendCommand(pause_command);
            return;
        }

        CalculateNewTransform(player_entity, input, delta_time);
    }

    void PlayerControllerSystem::CalculateNewTransform(const Engine::Ecs::EntityId player_entity,
                                                       const Engine::Input::InputBuffer& input,
                                                       const float delta_time) const
    {
        const auto transform = GameWorld()->GetComponent<Engine::Components::Transform>(player_entity);
        const auto rigidbody = GameWorld()->GetComponent<Engine::Components::Rigidbody>(player_entity);
        if (transform == nullptr || rigidbody == nullptr)
        {
            return;
        }
        const auto cam_rotation = transform->GetRotation();
        if (!m_initialized)
        {
            m_pitch_target = cam_rotation.x;
            m_yaw_target = cam_rotation.y;
            m_initialized = true;
        }

        // Calculate camera rotation
        const auto mouse_delta = input.mouse_delta;
        m_yaw_target += -mouse_delta.x * m_sensitivity; 
        m_pitch_target += -mouse_delta.y * m_sensitivity; 
        m_pitch_target = glm::clamp(m_pitch_target, m_min_pitch, m_max_pitch);

        float smoothing = 10.0f;
        float t = 1.0f - std::exp(-smoothing * delta_time);

        float pitch = glm::mix(cam_rotation.x, m_pitch_target, t);
        float yaw = glm::mix(cam_rotation.y, m_yaw_target, t);

        auto new_camera_rotation = glm::vec3(pitch, yaw, cam_rotation.z);
        transform->SetRotation(new_camera_rotation);

        // Calculate camera position
        const auto forward_vec = normalize(glm::vec3(sin(glm::radians(yaw)), 0.0f, cos(glm::radians(yaw))));
        const auto right_vec = normalize(cross(forward_vec, glm::vec3(0, 1, 0)));

        auto camera_displacement = glm::vec3(0.0f);
        if (input.HasAction("forward"))
        {
            camera_displacement.z -= 1;
        }
        if (input.HasAction("backwards"))
        {
            camera_displacement.z += 1;
        }
        if (input.HasAction("left"))
        {
            camera_displacement.x += 1;
        }
        if (input.HasAction("right"))
        {
            camera_displacement.x -= 1;
        }
        const glm::vec3 move_delta = right_vec * camera_displacement.x + forward_vec * camera_displacement.z;
        const glm::vec3 desired_velocity = move_delta * m_movement_speed;
        rigidbody->SetVelocity(desired_velocity);
    }
} // namespace
