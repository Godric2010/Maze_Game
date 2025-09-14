#include "GameplayManager.hpp"

#include "DebugGridDrawer.hpp"
#include "InputReceiver.hpp"
#include "Mesh.hpp"
#include "components/Camera.hpp"
#include "components/Transform.hpp"

namespace Gameplay {
    GameplayManager::GameplayManager(Engine::Core::IEngine &engine) : m_engine(engine) {
        m_objects.clear();
        m_camera_entity = Engine::Ecs::INVALID_ENTITY_ID;
        m_maze_algorithm = std::make_unique<Mazegenerator::MazeAlgorithm>(5, 5, 1337);
    }

    GameplayManager::~GameplayManager() = default;

    void GameplayManager::Initialize() {
        auto maze = m_maze_algorithm->GenerateMaze();
        const auto debug_grid_drawer = std::make_unique<DebugGridDrawer>();
        debug_grid_drawer->DrawGrid(maze);
        CreateCamera(maze.entrance_cell);
        CreateObjects(maze);
    }

    void GameplayManager::Shutdown() {
    }

    void GameplayManager::CreateCamera(const Mazegenerator::CellIndex &start_pos) {
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

        const auto camera_transform = Engine::Core::Components::Transform(glm::vec3(start_pos.x, 1.0f, start_pos.y),
                                                                          glm::vec3(-10.f, 180.0f, 0.0f));
        m_engine.GetWorld().AddComponent(m_camera_entity, camera_transform);


        constexpr auto input_receiver = Engine::Core::Components::InputReceiver{
            .Input = nullptr,
        };
        m_engine.GetWorld().AddComponent(m_camera_entity, input_receiver);
    }

    glm::vec4 GameplayManager::DetermineFloorColorForCell(const Mazegenerator::Maze &maze,
                                                          const Mazegenerator::CellIndex &cell_idx) {
        auto floor_color = glm::vec4{1.0f, 1.0f, 1.0f, 1.0f};
        if (cell_idx == maze.entrance_cell) {
            floor_color = glm::vec4{0.0f, 0.0f, 1.0f, 1.0f};
        }
        if (cell_idx == maze.exit_cell) {
            floor_color = glm::vec4{0.0f, 1.0f, 0.0f, 1.0f};
        }
        if (cell_idx == maze.key_cell) {
            floor_color = glm::vec4{1.0f, 1.0f, 0.0f, 1.0f};
        }
        return floor_color;
    }

    void GameplayManager::CreateCellFloorTile(const Engine::Renderer::MeshHandle mesh_handle,
                                              const Mazegenerator::CellIndex &cell_idx,
                                              const glm::vec4 &tile_color) const {
        const auto entity = m_engine.GetWorld().CreateEntity();
        const auto mesh_component = Engine::Core::Components::Mesh{
            .mesh = mesh_handle,
            .color = tile_color,
        };
        m_engine.GetWorld().AddComponent(entity, mesh_component);
        const auto position = glm::vec3(cell_idx.x, 0.0f, cell_idx.y);
        constexpr auto rotation = glm::vec3(0.0f, 0.0f, 0.0f);
        const auto transform_component = Engine::Core::Components::Transform(position, rotation);
        m_engine.GetWorld().AddComponent(entity, transform_component);
    }

    void GameplayManager::CreateWallFloorTile(const Engine::Renderer::MeshHandle mesh_handle,
                                              const Mazegenerator::CellIndex &cell_idx,
                                              const glm::vec4 &tile_color,
                                              const Mazegenerator::Direction &direction) const {
        const auto entity = m_engine.GetWorld().CreateEntity();
        const auto mesh_component = Engine::Core::Components::Mesh{
            .mesh = mesh_handle,
            .color = tile_color,
        };
        m_engine.GetWorld().AddComponent(entity, mesh_component);

        auto shift_vector = glm::vec3(0.0f);
        auto rotation_shift = glm::vec3(0.0f);
        switch (direction) {
            case Mazegenerator::Direction::back:
                shift_vector = glm::vec3(0.0f, 0.0f, -0.5f);
                break;
            case Mazegenerator::Direction::front:
                shift_vector = glm::vec3(0.0f, 0.0f, 0.5f);
                break;
            case Mazegenerator::Direction::left:
                shift_vector = glm::vec3(-0.5f, 0.0f, 0.0f);
                rotation_shift = glm::vec3(0.0f, 90.0f, 0.0f);
                break;
            case Mazegenerator::Direction::right:
                shift_vector = glm::vec3(0.5f, 0.0f, 0.0f);
                rotation_shift = glm::vec3(0.0f, 90.0f, 0.0f);
                break;
        }


        const auto position = glm::vec3(cell_idx.x, 0.5f, cell_idx.y) + shift_vector;
        const auto rotation = glm::vec3(0.0f, 0.0f, 0.0f) + rotation_shift;
        const auto transform_component = Engine::Core::Components::Transform(position, rotation);
        m_engine.GetWorld().AddComponent(entity, transform_component);
    }

    void GameplayManager::CreateObjects(const Mazegenerator::Maze &maze) const {
        const auto floor_mesh_handle = CreateFloorMesh();
        const auto wall_mesh_handle = CreateWallMesh();

        const auto maze_cells = maze.cells;
        for (const auto &cell: maze_cells) {
            CreateMazeCell(maze, cell, floor_mesh_handle, wall_mesh_handle);
        }

        const auto key_mesh_handle = CreateKeyMesh();
        CreateKeyObject(maze.key_cell, key_mesh_handle);
    }

    Engine::Renderer::MeshHandle GameplayManager::CreateFloorMesh() const {
        auto quad_mesh = Engine::Renderer::MeshAsset{};
        quad_mesh.vertices = std::vector<glm::vec3>();
        quad_mesh.vertices.emplace_back(-0.5, 0.0, -0.5);
        quad_mesh.vertices.emplace_back(0.5, 0.0, -0.5);
        quad_mesh.vertices.emplace_back(0.5, 0.0, 0.5);
        quad_mesh.vertices.emplace_back(-0.5, 0.0, 0.5);
        quad_mesh.indices = std::vector<unsigned int>();
        quad_mesh.indices.push_back(0);
        quad_mesh.indices.push_back(1);
        quad_mesh.indices.push_back(2);
        quad_mesh.indices.push_back(2);
        quad_mesh.indices.push_back(3);
        quad_mesh.indices.push_back(0);
        const Engine::Renderer::MeshHandle mesh_handle = m_engine.RegisterMesh(quad_mesh);
        return mesh_handle;
    }

    Engine::Renderer::MeshHandle GameplayManager::CreateWallMesh() const {
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
        const Engine::Renderer::MeshHandle mesh_handle = m_engine.RegisterMesh(quad_mesh);
        return mesh_handle;
    }

    Engine::Renderer::MeshHandle GameplayManager::CreateKeyMesh() const {
        auto cube_mesh = Engine::Renderer::MeshAsset{};
        cube_mesh.vertices = std::vector<glm::vec3>();
        cube_mesh.indices = std::vector<unsigned int>();

        // front face
        cube_mesh.vertices.emplace_back(-0.5, -0.5, -0.5);
        cube_mesh.vertices.emplace_back(0.5, -0.5, -0.5);
        cube_mesh.vertices.emplace_back(0.5, 0.5, -0.5);
        cube_mesh.vertices.emplace_back(-0.5, 0.5, -0.5);
        cube_mesh.indices.push_back(0);
        cube_mesh.indices.push_back(1);
        cube_mesh.indices.push_back(2);
        cube_mesh.indices.push_back(2);
        cube_mesh.indices.push_back(3);
        cube_mesh.indices.push_back(0);

        // back face
        cube_mesh.vertices.emplace_back(-0.5, -0.5, 0.5);
        cube_mesh.vertices.emplace_back(0.5, -0.5, 0.5);
        cube_mesh.vertices.emplace_back(0.5, 0.5, 0.5);
        cube_mesh.vertices.emplace_back(-0.5, 0.5, 0.5);
        cube_mesh.indices.push_back(4);
        cube_mesh.indices.push_back(5);
        cube_mesh.indices.push_back(6);
        cube_mesh.indices.push_back(6);
        cube_mesh.indices.push_back(7);
        cube_mesh.indices.push_back(4);

        // top face
        cube_mesh.vertices.emplace_back(-0.5, 0.5, -0.5);
        cube_mesh.vertices.emplace_back(0.5, 0.5, -0.5);
        cube_mesh.vertices.emplace_back(0.5, 0.5, 0.5);
        cube_mesh.vertices.emplace_back(-0.5, 0.5, -0.5);
        cube_mesh.indices.push_back(8);
        cube_mesh.indices.push_back(9);
        cube_mesh.indices.push_back(10);
        cube_mesh.indices.push_back(10);
        cube_mesh.indices.push_back(11);
        cube_mesh.indices.push_back(8);

        // bottom face
        cube_mesh.vertices.emplace_back(-0.5, -0.5, -0.5);
        cube_mesh.vertices.emplace_back(0.5, -0.5, -0.5);
        cube_mesh.vertices.emplace_back(0.5, -0.5, 0.5);
        cube_mesh.vertices.emplace_back(-0.5, -0.5, 0.5);
        cube_mesh.indices.push_back(12);
        cube_mesh.indices.push_back(13);
        cube_mesh.indices.push_back(14);
        cube_mesh.indices.push_back(14);
        cube_mesh.indices.push_back(15);
        cube_mesh.indices.push_back(12);

        // left face
        cube_mesh.vertices.emplace_back(-0.5, -0.5, 0.5);
        cube_mesh.vertices.emplace_back(-0.5, -0.5, -0.5);
        cube_mesh.vertices.emplace_back(-0.5, 0.5, -0.5);
        cube_mesh.vertices.emplace_back(-0.5, 0.5, 0.5);
        cube_mesh.indices.push_back(16);
        cube_mesh.indices.push_back(17);
        cube_mesh.indices.push_back(18);
        cube_mesh.indices.push_back(18);
        cube_mesh.indices.push_back(19);
        cube_mesh.indices.push_back(16);

        // right face
        cube_mesh.vertices.emplace_back(0.5, -0.5, -0.5);
        cube_mesh.vertices.emplace_back(0.5, -0.5, 0.5);
        cube_mesh.vertices.emplace_back(0.5, 0.5, 0.5);
        cube_mesh.vertices.emplace_back(0.5, 0.5, -0.5);
        cube_mesh.indices.push_back(20);
        cube_mesh.indices.push_back(21);
        cube_mesh.indices.push_back(22);
        cube_mesh.indices.push_back(22);
        cube_mesh.indices.push_back(23);
        cube_mesh.indices.push_back(20);

        const auto mesh_handle = m_engine.RegisterMesh(cube_mesh);
        return mesh_handle;
    }

    void GameplayManager::CreateKeyObject(const Mazegenerator::CellIndex &cell_index,
                                          const Engine::Renderer::MeshHandle &key_mesh_handle) const {
        const auto entity = m_engine.GetWorld().CreateEntity();
        const auto mesh_component = Engine::Core::Components::Mesh{
            .mesh = key_mesh_handle,
            .color = {0.3, 1.0, 1.0, 1.0},
        };
        m_engine.GetWorld().AddComponent(entity, mesh_component);
        const auto position = glm::vec3(cell_index.x, 0.5f, cell_index.y);
        constexpr auto rotation = glm::vec3(0.0f, 0.0f, 0.0f);
        constexpr auto scale = glm::vec3(0.2f, 0.2f, 0.2f);
        const auto transform_component = Engine::Core::Components::Transform(position, rotation, scale);
        m_engine.GetWorld().AddComponent(entity, transform_component);
    }

    void GameplayManager::CreateMazeCell(const Mazegenerator::Maze &maze, const Mazegenerator::Cell &cell,
                                         const Engine::Renderer::MeshHandle &floor_mesh_handle,
                                         const Engine::Renderer::MeshHandle &wall_mesh_handle) const {
        const auto floor_tile_color = DetermineFloorColorForCell(maze, cell.cell_index);
        CreateCellFloorTile(floor_mesh_handle, cell.cell_index, floor_tile_color);

        constexpr auto wall_tile_color = glm::vec4{0.8f, 0.8f, 0.8f, 1.0f};

        if (cell.HasWall(Mazegenerator::front)) {
            CreateWallFloorTile(wall_mesh_handle, cell.cell_index, wall_tile_color, Mazegenerator::Direction::front);
        }
        if (cell.HasWall(Mazegenerator::back)) {
            CreateWallFloorTile(wall_mesh_handle, cell.cell_index, wall_tile_color, Mazegenerator::Direction::back);
        }
        if (cell.HasWall(Mazegenerator::left)) {
            CreateWallFloorTile(wall_mesh_handle, cell.cell_index, wall_tile_color, Mazegenerator::Direction::left);
        }
        if (cell.HasWall(Mazegenerator::right)) {
            CreateWallFloorTile(wall_mesh_handle, cell.cell_index, wall_tile_color, Mazegenerator::Direction::right);
        }
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
