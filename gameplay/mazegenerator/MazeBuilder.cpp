#include "MazeBuilder.hpp"

#include <ostream>

#include "Collider.hpp"
#include "../../engine/components/Mesh.hpp"
#include "../../engine/components/Transform.hpp"
#include "../components/Exit.hpp"
#include "../components/KeyItem.hpp"

namespace Gameplay::Mazegenerator {
    MazeBuilder::MazeBuilder(Engine::SceneManagement::SceneWorld* game_world,
                             const Engine::Renderer::MeshHandle floor_mesh,
                             const Engine::Renderer::MeshHandle wall_mesh,
                             const Engine::Renderer::MeshHandle key_mesh,
                             const bool enable_debug_view) : m_maze() {
        m_game_world = game_world;
        if (enable_debug_view) {
            m_debug_grid_drawer = std::make_unique<DebugGridDrawer>();
        }

        m_floor_mesh = floor_mesh;
        m_wall_mesh = wall_mesh;
        m_key_mesh = key_mesh;
    }

    void MazeBuilder::BuildMaze(int width, int height, int seed) {
        m_maze_algorithm = std::make_unique<MazeAlgorithm>(width, height, seed);
        m_maze = m_maze_algorithm->GenerateMaze();
        if (m_debug_grid_drawer) {
            m_debug_grid_drawer->DrawGrid(m_maze);
        }

        CreateCellObjects();
        CreateKeyObject(m_maze.key_cell);
        CreateExitTrigger(m_maze.exit_cell);
    }

    glm::vec3 MazeBuilder::GetMazeStartPosition() const {
        return {m_maze.entrance_cell.x, 1.0f, m_maze.entrance_cell.y};
    }


    void MazeBuilder::CreateCellObjects() const {
        const auto maze_cells = m_maze.cells;
        for (const auto& cell: maze_cells) {
            CreateMazeCell(cell);
        }
    }


    void MazeBuilder::CreateKeyObject(const CellIndex& cell_index) const {
        const auto entity = m_game_world->CreateEntity("KeyItem");
        const auto mesh_component = Engine::Components::Mesh{
            .mesh = m_key_mesh,
            .color = {0.3, 1.0, 1.0, 1.0},
        };
        m_game_world->AddComponent(entity, mesh_component);
        const auto position = glm::vec3(cell_index.x, 0.5f, cell_index.y);
        constexpr auto rotation = glm::vec3(0.0f, 0.0f, 0.0f);
        constexpr auto scale = glm::vec3(0.2f, 0.2f, 0.2f);
        const auto transform_component = Engine::Components::Transform()
                .SetPosition(position)
                .SetRotation(rotation)
                .SetScale(scale);
        m_game_world->AddComponent(entity, transform_component);

        constexpr auto collider = Engine::Components::BoxCollider{
            .is_static = true,
            .width = 0.2f,
            .height = 1.0f,
            .depth = 0.2f
        };
        m_game_world->AddComponent(entity, collider);

        m_game_world->AddComponent(entity, Components::KeyItem{});
    }

    void MazeBuilder::CreateExitTrigger(const CellIndex& cell_index) const {
        const auto entity = m_game_world->CreateEntity("ExitTrigger");

        const auto position = glm::vec3(cell_index.x, 0.5f, cell_index.y);
        constexpr auto rotation = glm::vec3(0.0f, 0.0f, 0.0f);
        constexpr auto scale = glm::vec3(1.0f);
        const auto transform_component = Engine::Components::Transform()
                .SetPosition(position)
                .SetRotation(rotation)
                .SetScale(scale);
        m_game_world->AddComponent(entity, transform_component);

        constexpr auto collider = Engine::Components::BoxCollider{
            .is_static = true,
            .is_trigger = true,
            .width = 0.2f,
            .height = 1.0f,
            .depth = 0.2f
        };
        m_game_world->AddComponent(entity, collider);

        m_game_world->AddComponent(entity, Components::Exit{});
    }


    void MazeBuilder::CreateMazeCell(const Cell& cell) const {
        const auto floor_tile_color = DetermineFloorColorForCell(cell.cell_index);
        CreateCellFloorTile(cell.cell_index, floor_tile_color);

        constexpr auto wall_tile_color = glm::vec4{0.8f, 0.8f, 0.8f, 1.0f};

        if (cell.HasWall(Front)) {
            CreateWallFloorTile(cell.cell_index, wall_tile_color, Front);
        }
        if (cell.HasWall(Back)) {
            CreateWallFloorTile(cell.cell_index, wall_tile_color, Back);
        }
        if (cell.HasWall(Left)) {
            CreateWallFloorTile(cell.cell_index, wall_tile_color, Left);
        }
        if (cell.HasWall(Right)) {
            CreateWallFloorTile(cell.cell_index, wall_tile_color, Right);
        }
    }

    void MazeBuilder::CreateCellFloorTile(
            const CellIndex& cell_idx,
            const glm::vec4& tile_color) const {
        const auto entity = m_game_world->CreateEntity(std::format("FloorTile [{}|{}]", cell_idx.x, cell_idx.y));
        const auto mesh_component = Engine::Components::Mesh{
            .mesh = m_floor_mesh,
            .color = tile_color,
        };
        m_game_world->AddComponent(entity, mesh_component);
        const auto position = glm::vec3(cell_idx.x, 0.0f, cell_idx.y);
        constexpr auto rotation = glm::vec3(0.0f, 0.0f, 0.0f);
        const auto transform_component = Engine::Components::Transform()
                .SetPosition(position)
                .SetRotation(rotation);
        m_game_world->AddComponent(entity, transform_component);
    }

    void MazeBuilder::CreateWallFloorTile(const CellIndex& cell_idx, const glm::vec4& tile_color,
                                          const Direction& direction) const {
        const auto entity = m_game_world->CreateEntity(
                std::format("WallTile [{}|{}]-{}", cell_idx.x, cell_idx.y, static_cast<int>(direction))
                );
        const auto mesh_component = Engine::Components::Mesh{
            .mesh = m_wall_mesh,
            .color = tile_color,
        };
        m_game_world->AddComponent(entity, mesh_component);

        auto shift_vector = glm::vec3(0.0f);
        auto rotation_shift = glm::vec3(0.0f);
        switch (direction) {
            case Back:
                shift_vector = glm::vec3(0.0f, 0.0f, -0.5f);
                break;
            case Front:
                shift_vector = glm::vec3(0.0f, 0.0f, 0.5f);
                break;
            case Left:
                shift_vector = glm::vec3(-0.5f, 0.0f, 0.0f);
                rotation_shift = glm::vec3(0.0f, 90.0f, 0.0f);
                break;
            case Right:
                shift_vector = glm::vec3(0.5f, 0.0f, 0.0f);
                rotation_shift = glm::vec3(0.0f, 90.0f, 0.0f);
                break;
        }

        const auto position = glm::vec3(cell_idx.x, 0.5f, cell_idx.y) + shift_vector;
        const auto rotation = glm::vec3(0.0f, 0.0f, 0.0f) + rotation_shift;
        const auto transform_component = Engine::Components::Transform()
                .SetPosition(position)
                .SetRotation(rotation);
        m_game_world->AddComponent(entity, transform_component);

        constexpr auto collider = Engine::Components::BoxCollider{
            .is_static = true,
            .width = 1.0f,
            .height = 1.0f,
            .depth = 1e-6f
        };
        m_game_world->AddComponent(entity, collider);
    }

    glm::vec4 MazeBuilder::DetermineFloorColorForCell(const CellIndex& cell_idx) const {
        auto floor_color = glm::vec4{1.0f, 1.0f, 1.0f, 1.0f};
        if (cell_idx == m_maze.entrance_cell) {
            floor_color = glm::vec4{0.0f, 0.0f, 1.0f, 1.0f};
        }
        if (cell_idx == m_maze.exit_cell) {
            floor_color = glm::vec4{0.0f, 1.0f, 0.0f, 1.0f};
        }
        if (cell_idx == m_maze.key_cell) {
            floor_color = glm::vec4{1.0f, 1.0f, 0.0f, 1.0f};
        }
        return floor_color;
    }
} // namespace
