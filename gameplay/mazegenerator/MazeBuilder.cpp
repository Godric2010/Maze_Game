#include "MazeBuilder.hpp"

#include <format>

#include "Collider.hpp"
#include "MeshRenderer.hpp"
#include "Transform.hpp"
#include "../components/Exit.hpp"
#include "../components/KeyItem.hpp"
#include "Assets/IAssetLibrary.hpp"

namespace Gameplay::Mazegenerator
{
    MazeBuilder::MazeBuilder(Engine::SceneManagement::SceneWorld* game_world,
                             Engine::Assets::IAssetLibrary* renderer,
                             const bool enable_debug_view) : m_maze()
    {
        m_game_world = game_world;
        m_assets = renderer;
        if (enable_debug_view)
        {
            m_debug_grid_drawer = std::make_unique<DebugGridDrawer>();
        }

        m_floor_mesh = m_assets->LoadMesh("FloorTile.obj");
        m_wall_mesh = m_assets->LoadMesh("WallTile.obj");
        m_key_mesh = m_assets->LoadMesh("Key.obj");
        m_default_material = m_assets->LoadMaterial("default.material");
        m_key_material = m_assets->LoadMaterial("key.material");
        m_start_material = m_assets->LoadMaterial("start_tile.material");
        m_exit_material = m_assets->LoadMaterial("exit_tile.material");
        m_wall_material = m_assets->LoadMaterial("wall.material");
    }

    void MazeBuilder::BuildMaze(int width, int height, int seed)
    {
        m_maze_algorithm = std::make_unique<MazeAlgorithm>(width, height, seed);
        m_maze = m_maze_algorithm->GenerateMaze();
        if (m_debug_grid_drawer)
        {
            m_debug_grid_drawer->DrawGrid(m_maze);
        }

        CreateCellObjects();
        CreateKeyObject(m_maze.key_cell);
        CreateExitTrigger(m_maze.exit_cell);
    }

    glm::vec3 MazeBuilder::GetMazeStartPosition() const
    {
        return {m_maze.entrance_cell.x, 1.0f, m_maze.entrance_cell.y};
    }


    void MazeBuilder::CreateCellObjects() const
    {
        const auto maze_cells = m_maze.cells;
        for (const auto& cell : maze_cells)
        {
            CreateMazeCell(cell);
        }
    }


    void MazeBuilder::CreateKeyObject(const CellIndex& cell_index) const
    {
        const auto entity = m_game_world->CreateEntity("KeyItem");
        const auto mesh_component = Engine::Components::MeshRenderer{
            .Mesh = m_key_mesh,
            .Material = m_key_material,
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

    void MazeBuilder::CreateExitTrigger(const CellIndex& cell_index) const
    {
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


    void MazeBuilder::CreateMazeCell(const Cell& cell) const
    {
        const auto tile_material = DetermineFloorMaterialForCell(cell.cell_index);
        CreateCellFloorTile(cell.cell_index, tile_material);

        if (cell.HasWall(Front))
        {
            CreateWallTile(cell.cell_index, Front);
        }
        if (cell.HasWall(Back))
        {
            CreateWallTile(cell.cell_index, Back);
        }
        if (cell.HasWall(Left))
        {
            CreateWallTile(cell.cell_index, Left);
        }
        if (cell.HasWall(Right))
        {
            CreateWallTile(cell.cell_index, Right);
        }
    }

    void MazeBuilder::CreateCellFloorTile(
        const CellIndex& cell_idx,
        Engine::Assets::MaterialHandle material) const
    {
        const auto entity = m_game_world->CreateEntity(std::format("FloorTile [{}|{}]", cell_idx.x, cell_idx.y));
        const auto mesh_component = Engine::Components::MeshRenderer{
            .Mesh = m_floor_mesh,
            .Material = material,
        };
        m_game_world->AddComponent(entity, mesh_component);
        const auto position = glm::vec3(cell_idx.x, 0.0f, cell_idx.y);
        constexpr auto rotation = glm::vec3(0.0f, 0.0f, 0.0f);
        const auto transform_component = Engine::Components::Transform()
                                         .SetPosition(position)
                                         .SetRotation(rotation);
        m_game_world->AddComponent(entity, transform_component);
    }

    void MazeBuilder::CreateWallTile(const CellIndex& cell_idx,
                                     const Direction& direction) const
    {
        const auto entity = m_game_world->CreateEntity(
            std::format("WallTile [{}|{}]-{}", cell_idx.x, cell_idx.y, static_cast<int>(direction))
        );

        auto shift_vector = glm::vec3(0.0f);
        auto rotation_shift = glm::vec3(0.0f);
        switch (direction)
        {
            case Back:
                shift_vector = glm::vec3(0.0f, 0.0f, -0.5f);
                rotation_shift = glm::vec3(0.0f, 180.0f, 0.0f);
                break;
            case Front:
                shift_vector = glm::vec3(0.0f, 0.0f, 0.5f);
                break;
            case Left:
                shift_vector = glm::vec3(-0.5f, 0.0f, 0.0f);
                rotation_shift = glm::vec3(0.0f, -90.0f, 0.0f);
                break;
            case Right:
                shift_vector = glm::vec3(0.5f, 0.0f, 0.0f);
                rotation_shift = glm::vec3(0.0f, 90.0f, 0.0f);
                break;
        }

        const auto mesh_component = Engine::Components::MeshRenderer{
            .Mesh = m_wall_mesh,
            .Material = m_wall_material,
        };

        m_game_world->AddComponent(entity, mesh_component);
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

    Engine::Assets::MaterialHandle MazeBuilder::DetermineFloorMaterialForCell(const CellIndex& cell_idx) const
    {
        auto material = m_default_material;
        if (cell_idx == m_maze.entrance_cell)
        {
            material = m_start_material;
        }
        if (cell_idx == m_maze.exit_cell)
        {
            material = m_exit_material;
        }
        // if (cell_idx == m_maze.key_cell)
        // {
        //     material = glm::vec4{1.0f, 1.0f, 0.0f, 1.0f};
        // }
        return material;
    }
} // namespace
