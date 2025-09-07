#include "GameplayManager.hpp"

#include "InputReceiver.hpp"
#include "Mesh.hpp"
#include "components/Camera.hpp"
#include "components/Transform.hpp"

namespace Gameplay {
    GameplayManager::GameplayManager(Engine::Core::IEngine &iEngine) : IEngine(iEngine) {
        m_objects.clear();
        m_cameraEntity = Engine::Ecs::INVALID_ENTITY_ID;
        m_maze_algorithm = std::make_unique<Mazegenerator::MazeAlgorithm>(5, 5, 1337);
    }

    GameplayManager::~GameplayManager() = default;

    void GameplayManager::Initialize() {
        auto maze = m_maze_algorithm->GenerateMaze();
        createCamera();
        createObjects();
    }

    void GameplayManager::Shutdown() {
    }

    void GameplayManager::createCamera() {
        m_cameraEntity = IEngine.GetWorld().CreateEntity();
        constexpr auto cameraComponent = Engine::Core::Components::Camera{
            .Width = 1920, // TODO: Use direct data from config later
            .Height = 1080,
            .FieldOfView = 60,
            .AspectRatio = 1920.0f / 1080.0f,
            .NearClip = 0.01f,
            .FarClip = 1000.0f,

        };
        IEngine.GetWorld().AddComponent<Engine::Core::Components::Camera>(m_cameraEntity, cameraComponent);

        constexpr auto cameraTransform = Engine::Core::Components::Transform{
            .Position = glm::vec3(0.0f, 0.0f, 3.0f),
            .Rotation = glm::vec3(1.0f, 0.0f, 0.0f),
            .Scale = glm::vec3(1.0f, 1.0f, 1.0f),
        };
        IEngine.GetWorld().AddComponent(m_cameraEntity, cameraTransform);


        constexpr auto inputReceiver = Engine::Core::Components::InputReceiver{
            .Input = nullptr,
        };
        IEngine.GetWorld().AddComponent(m_cameraEntity, inputReceiver);
    }

    void GameplayManager::createObjects() const {
        auto quad_mesh = Engine::Renderer::MeshAsset{};
        quad_mesh.vertices = std::vector<glm::vec3>();
        quad_mesh.vertices.emplace_back(-0.5, -0.5, 0.0);
        quad_mesh.vertices.emplace_back(0.5, -0.5, 0.0);
        quad_mesh.vertices.emplace_back(0.5, 0.5, 0.0);
        quad_mesh.vertices.emplace_back(-0.5, 0.5, 0.0);
        quad_mesh.indices = std::vector<unsigned int>();
        quad_mesh.indices.push_back(0);
        quad_mesh.indices.push_back(1);
        quad_mesh.indices.push_back(2);
        quad_mesh.indices.push_back(2);
        quad_mesh.indices.push_back(3);
        quad_mesh.indices.push_back(0);
        const Engine::Renderer::MeshHandle meshHandle = IEngine.RegisterMesh(quad_mesh);


        auto entityA = IEngine.GetWorld().CreateEntity();
        auto transformA = Engine::Core::Components::Transform{
            .Position = glm::vec3(0.0f, 0.0f, -2.0f),
            .Rotation = glm::vec3(10.0f, 30.0f, 0.0f),
            .Scale = glm::vec3(1.0f, 1.0f, 1.0f),
            .Matrix = glm::mat4(1.0f),
        };
        IEngine.GetWorld().AddComponent(entityA, transformA);
        auto meshA = Engine::Core::Components::Mesh{
            .mesh = meshHandle,
            .color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),
        };
        IEngine.GetWorld().AddComponent(entityA, meshA);

        auto entityB = IEngine.GetWorld().CreateEntity();
        auto transformB = Engine::Core::Components::Transform{
            .Position = glm::vec3(0.5f, 0.0f, -1.0f),
            .Rotation = glm::vec3(0.0f, -10.0f, 0.0f),
            .Scale = glm::vec3(1.0f, 1.0f, 1.0f),
            .Matrix = glm::mat4(1.0f),
        };
        IEngine.GetWorld().AddComponent(entityB, transformB);
        auto meshB = Engine::Core::Components::Mesh{
            .mesh = meshHandle,
            .color = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f),
        };
        IEngine.GetWorld().AddComponent(entityB, meshB);
    }
} // namespace
