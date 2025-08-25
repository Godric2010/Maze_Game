#include "GameplayManager.hpp"

#include "InputReceiver.hpp"
#include "components/Camera.hpp"
#include "components/Transform.hpp"

namespace Gameplay {
    GameplayManager::GameplayManager(Engine::Core::EngineController *engineController) {
        EngineController = engineController;
        m_objects.clear();
        m_cameraEntity = Engine::Ecs::INVALID_ENTITY_ID;
    }

    GameplayManager::~GameplayManager() {
        EngineController = nullptr;
    }

    void GameplayManager::Initialize() {
        createCamera();
        createObjects();
    }

    void GameplayManager::Shutdown() {
    }

    void GameplayManager::createCamera() {
        m_cameraEntity = EngineController->GetWorld().CreateEntity();
        constexpr auto cameraComponent = Engine::Core::Components::Camera{
            .Width = 1920, // TODO: Use direct data from config later
            .Height = 1080,
            .FieldOfView = 60,
            .AspectRatio = 1920.0f / 1080.0f,
            .NearClip = 0.01f,
            .FarClip = 1000.0f,

        };
        EngineController->GetWorld().AddComponent<Engine::Core::Components::Camera>(m_cameraEntity, cameraComponent);

        constexpr auto cameraTransform = Engine::Core::Components::Transform{
            .Position = glm::vec3(0.0f, 0.0f, 3.0f),
            .Rotation = glm::vec3(0.0f, 0.0f, 0.0f),
            .Scale = glm::vec3(1.0f, 1.0f, 1.0f),
        };
        EngineController->GetWorld().AddComponent(m_cameraEntity, cameraTransform);


        constexpr auto inputReceiver = Engine::Core::Components::InputReceiver{
            .Input = nullptr,
        };
        EngineController->GetWorld().AddComponent(m_cameraEntity, inputReceiver);

    }

    void GameplayManager::createObjects() {
    }
} // namespace
