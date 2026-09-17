#include "PlayerControllerSystem.hpp"

#include <iostream>
#include <ostream>

#include "Rigidbody.hpp"
#include "SystemWorld.hpp"
#include "Transform.hpp"
#include "../commands/PauseCommand.hpp"
#include "../components/FirstPersonControllerComponent.hpp"

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
        const auto fps_controller = GameWorld()->GetComponent<
            components::FirstPersonControllerComponent>(player_entity);
        if (transform == nullptr || rigidbody == nullptr || fps_controller == nullptr) {
            return;
        }

        // Set yaw/pitch targets
        const auto mouse_delta = input.mouse_delta;
        auto yaw_target = fps_controller->GetYaw() +
                          yarep::math::Angle::from_degrees(-mouse_delta.x * m_sensitivity);
        auto pitch_target = fps_controller->GetPitch() +
                            yarep::math::Angle::from_degrees(-mouse_delta.y * m_sensitivity);

        pitch_target = yarep::math::Angle::from_radians(
                std::clamp(pitch_target.radians(), m_min_pitch.radians(), m_max_pitch.radians())
                );

        fps_controller->SetTargetYaw(yaw_target).SetTargetPitch(pitch_target);


        // Smoothing
        constexpr float smoothing = 10.0f;
        const float t = 1.0f - std::exp(-smoothing * delta_time);

        const auto pitch = yarep::math::lerp(fps_controller->GetPitch(), pitch_target, t);
        const auto yaw = yarep::math::lerp(fps_controller->GetYaw(), yaw_target, t);

        fps_controller->SetYaw(yaw).SetPitch(pitch);

        // Write final runtime rotation
        const auto new_camera_rotation = yarep::math::from_yaw_pitch_roll(yaw, pitch, yarep::math::Angle{});
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
