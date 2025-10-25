#include "GameplayManager.hpp"

#include "Collider.hpp"
#include "InputReceiver.hpp"
#include "Mesh.hpp"
#include "MotionIntent.hpp"
#include "components/Camera.hpp"
#include "components/Inventory.hpp"
#include "components/Transform.hpp"

namespace Gameplay {
    GameplayManager::GameplayManager(Engine::Core::IEngine &engine) : m_engine(engine) {
        m_camera_entity = Engine::Ecs::INVALID_ENTITY_ID;
        m_mesh_handler = std::make_unique<MeshHandler>(engine);
        m_maze_builder = std::make_unique<Mazegenerator::MazeBuilder>(&engine.GetWorld(),
                                                                      m_mesh_handler->GetFloorMesh(),
                                                                      m_mesh_handler->GetWallMesh(),
                                                                      m_mesh_handler->GetKeyMesh(),
                                                                      true);
    }

    GameplayManager::~GameplayManager() = default;

    void GameplayManager::Initialize() {
        m_maze_builder->BuildMaze(5, 5, 1337);
        CreateCamera(m_maze_builder->GetMazeStartPosition());
    }

    void GameplayManager::Shutdown() {
    }

    void GameplayManager::CreateCamera(glm::vec3 start_pos) {
        m_camera_entity = m_engine.GetWorld().CreateEntity();
        constexpr auto camera_component = Engine::Core::Components::Camera{
            .Width = 1920, // TODO: Use direct data from config later
            .Height = 1080,
            .FieldOfView = 60,
            .AspectRatio = 1920.0f / 1080.0f,
            .NearClip = 0.01f,
            .FarClip = 1000.0f,

        };
        m_engine.GetWorld().AddComponent<Engine::Core::Components::Camera>(m_camera_entity, camera_component);

        const auto camera_transform = Engine::Core::Components::Transform(start_pos,
                                                                          glm::vec3(-10.f, 180.0f, 0.0f));
        m_engine.GetWorld().AddComponent(m_camera_entity, camera_transform);
        const auto camera_motion_intent = Engine::Core::Components::MotionIntent();
        m_engine.GetWorld().AddComponent(m_camera_entity, camera_motion_intent);

        constexpr auto camera_collider = Engine::Core::Components::SphereCollider{
            .is_static = false, .radius = 0.1f
        };
        m_engine.GetWorld().AddComponent(m_camera_entity, camera_collider);


        constexpr auto input_receiver = Engine::Core::Components::InputReceiver{
            .Input = nullptr,
        };
        m_engine.GetWorld().AddComponent(m_camera_entity, input_receiver);

        constexpr auto inventory = Components::Inventory();
        m_engine.GetWorld().AddComponent(m_camera_entity, inventory);
    }

    glm::vec3 GameplayManager::ConvertDirection(const Mazegenerator::Direction &direction) {
        switch (direction) {
            case Mazegenerator::Direction::back:
                return {0.0f, 0.0f, -1.0f};
            case Mazegenerator::Direction::front:
                return {0.0f, 0.0f, 1.0f};
            case Mazegenerator::Direction::left:
                return {-1.0f, 0.0f, 0.0f};
            case Mazegenerator::Direction::right:
                return {1.0f, 0.0f, 0.0f};
            default:
                return {0.0f, 0.0f, 0.0f};
        }
    }
} // namespace
