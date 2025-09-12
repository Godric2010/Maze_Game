#include "CameraControllerSystem.hpp"


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
            const auto cameraTransform = world.GetComponent<Engine::Core::Components::Transform>(entity);
            const auto cameraInput = world.GetComponent<Engine::Core::Components::InputReceiver>(entity);
            CalculateNewTransform(*cameraTransform, cameraInput->Input, deltaTime);
        }
    }

    void CameraControllerSystem::CalculateNewTransform(Engine::Core::Components::Transform &transform,
                                                       const Engine::Environment::InputSnapshot *input,
                                                       const float deltaTime) const {
        auto cameraPos = transform.position;
        if (input->IsKeyHeld(Engine::Environment::Key::W)) {
            cameraPos.z -= m_velocity * deltaTime;
        }
        if (input->IsKeyHeld(Engine::Environment::Key::S)) {
            cameraPos.z += m_velocity * deltaTime;
        }
        if (input->IsKeyHeld(Engine::Environment::Key::A)) {
            cameraPos.x -= m_velocity * deltaTime;
        }
        if (input->IsKeyHeld(Engine::Environment::Key::D)) {
            cameraPos.x += m_velocity * deltaTime;
        }
        transform.position = cameraPos;

        const auto mouseDelta = input->GetMouseDelta();
        const float yawDelta = mouseDelta.x * m_sensitivity;
        const float pitchDelta = mouseDelta.y * m_sensitivity;

        const float pitch = glm::clamp(transform.rotation.x + pitchDelta, m_minPitch, m_maxPitch);
        transform.rotation = glm::vec3(pitch, transform.rotation.y + yawDelta, transform.rotation.z);
    }
} // namespace
