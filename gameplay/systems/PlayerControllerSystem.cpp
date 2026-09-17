#include "PlayerControllerSystem.hpp"

#include <iostream>
#include <ostream>

#include "Rigidbody.hpp"
#include "SystemWorld.hpp"
#include "Transform.hpp"
#include "../commands/PauseCommand.hpp"

namespace gameplay::systems {
    PlayerControllerSystem::PlayerControllerSystem() = default;

    PlayerControllerSystem::~PlayerControllerSystem() = default;

    void PlayerControllerSystem::Initialize() {
    }

    void PlayerControllerSystem::Run(const float delta_time) {
        const auto input = Input()->GetInput();
        if (!input.IsMapActive("PlayerInputMap"))
            return;

        const auto player_entity = GameWorld()->GetEntityByName("Player");

        if (input.HasAction("pause")) {
            std::cout << "Enabled pause!" << std::endl;

            const auto pause_command = commands::PauseCommand(true);
            SendCommand(pause_command);
            return;
        }

        CalculateNewTransform(player_entity, input, delta_time);
    }

    void PlayerControllerSystem::CalculateNewTransform(const yarep::ecs::EntityId player_entity,
                                                       const yarep::input::InputBuffer& input,
                                                       const float delta_time) const {
        const auto transform = GameWorld()->GetComponent<yarep::components::TransformComponent>(player_entity);
        const auto rigidbody = GameWorld()->GetComponent<yarep::components::Rigidbody>(player_entity);
        if (transform == nullptr || rigidbody == nullptr) {
            return;
        }
        const auto cam_rotation = transform->GetRotation();
        const auto cam_rotation_x = yarep::math::Angle::from_degrees(cam_rotation.x);
        const auto cam_rotation_y = yarep::math::Angle::from_degrees(cam_rotation.y);
        if (!m_initialized) {
            m_pitch_target = cam_rotation_x;
            m_yaw_target = cam_rotation_y;
            m_initialized = true;
        }

        // Calculate camera rotation
        const auto mouse_delta = input.mouse_delta;
        m_yaw_target = m_yaw_target + yarep::math::Angle::from_degrees(-mouse_delta.x * m_sensitivity);
        m_pitch_target = m_pitch_target + yarep::math::Angle::from_degrees(-mouse_delta.y * m_sensitivity);
        m_pitch_target = yarep::math::Angle::from_radians(std::clamp(m_pitch_target.radians(),
                                                                     m_min_pitch.radians(),
                                                                     m_max_pitch.radians()
                        )
                );

        float smoothing = 10.0f;
        float t = 1.0f - std::exp(-smoothing * delta_time);

        auto pitch = yarep::math::lerp(cam_rotation_x, m_pitch_target, t);
        auto yaw = yarep::math::lerp(cam_rotation_y, m_yaw_target, t);

        auto new_camera_rotation = glm::vec3(pitch.degrees(), yaw.degrees(), cam_rotation.z);
        transform->SetRotation(new_camera_rotation);

        // Calculate camera position
        const auto forward_vec = normalize(yarep::math::Vec3(sin(yaw.radians()), 0.0f, cos(yaw.radians())));
        const auto right_vec = normalize(cross(forward_vec, yarep::math::Vec3(0, 1, 0)));

        auto camera_displacement = yarep::math::Vec3{};
        if (input.HasAction("forward")) {
            camera_displacement.z -= 1;
        }
        if (input.HasAction("backwards")) {
            camera_displacement.z += 1;
        }
        if (input.HasAction("left")) {
            camera_displacement.x += 1;
        }
        if (input.HasAction("right")) {
            camera_displacement.x -= 1;
        }
        const yarep::math::Vec3 move_delta = right_vec * camera_displacement.x + forward_vec * camera_displacement.z;
        const yarep::math::Vec3 desired_velocity = move_delta * m_movement_speed;
        rigidbody->SetVelocity(desired_velocity);
    }
} // namespace
