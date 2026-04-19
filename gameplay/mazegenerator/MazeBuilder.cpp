#include "MazeBuilder.hpp"

#include <format>

#include "Collider.hpp"
#include "MeshRenderer.hpp"
#include "Transform.hpp"
#include "../components/Exit.hpp"
#include "../components/KeyItem.hpp"
#include "Assets/IAssetLibrary.hpp"

namespace Gameplay::Mazegenerator {
    MazeBuilder::MazeBuilder(Engine::SceneManagement::SceneWorld* game_world,
                             Engine::Assets::IAssetLibrary* renderer,
                             const bool enable_debug_view) : m_maze() {
        m_game_world = game_world;
        m_assets = renderer;
        if (enable_debug_view) {
            m_debug_grid_drawer = std::make_unique<DebugGridDrawer>();
        }

        m_floor_mesh = m_assets->LoadMesh("FloorTiles.obj");
        m_wall_mesh = m_assets->LoadMesh("WallTile.obj");
        m_key_mesh = m_assets->LoadMesh("Key.obj");
        m_ceiling_mesh = m_assets->LoadMesh("Ceiling.obj");
        m_door_frame = m_assets->LoadMesh("DoorFrame.obj");
        m_door = m_assets->LoadMesh("Door.obj");
        m_default_material = m_assets->LoadMaterial("default.material");
        m_key_material = m_assets->LoadMaterial("key.material");
        m_start_material = m_assets->LoadMaterial("start_tile.material");
        m_exit_material = m_assets->LoadMaterial("exit_tile.material");
        m_wall_material = m_assets->LoadMaterial("wall.material");
        m_ceiling_material = m_assets->LoadMaterial("ceiling.material");
        m_door_material = m_assets->LoadMaterial("door.material");
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
        return {m_maze.entrance_cell.x * 2, 1.1f, m_maze.entrance_cell.y * 2};
    }


    void MazeBuilder::CreateCellObjects() const {
        const auto maze_cells = m_maze.cells;
        for (const auto& cell: maze_cells) {
            if (cell.cell_index == m_maze.exit_cell) {
                CreateExitCell(cell);
                continue;
            }

            CreateMazeCell(cell);
        }
    }

    void MazeBuilder::CreateExitCell(const Cell& exit_cell) const {
        CreateCellFloorTile(exit_cell.cell_index, m_exit_material);
        CreateCeilingTile(exit_cell.cell_index);

        if (exit_cell.HasWall(Front)) {
            CreateWallTile(exit_cell.cell_index, Front);
        } else {
            CreateDoorTile(exit_cell.cell_index, Front);
        }
        if (exit_cell.HasWall(Back)) {
            CreateWallTile(exit_cell.cell_index, Back);
        } else {
            CreateDoorTile(exit_cell.cell_index, Back);
        }
        if (exit_cell.HasWall(Left)) {
            CreateWallTile(exit_cell.cell_index, Left);
        } else {
            CreateDoorTile(exit_cell.cell_index, Left);
        }
        if (exit_cell.HasWall(Right)) {
            CreateWallTile(exit_cell.cell_index, Right);
        } else {
            CreateDoorTile(exit_cell.cell_index, Right);
        }
    }


    void MazeBuilder::CreateKeyObject(const CellIndex& cell_index) const {
        const auto entity = m_game_world->CreateEntity("KeyItem");
        const auto mesh_component = Engine::Components::MeshRenderer{
            .Mesh = m_key_mesh,
            .Material = m_key_material,
        };
        m_game_world->AddComponent(entity, mesh_component);
        const auto position = glm::vec3(cell_index.x * 2, 0.5f, cell_index.y * 2);
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

        const auto position = glm::vec3(cell_index.x * 2, 0.5f, cell_index.y * 2);
        constexpr auto rotation = glm::vec3(0.0f, 0.0f, 0.0f);
        constexpr auto scale = glm::vec3(2.0f);
        const auto transform_component = Engine::Components::Transform()
                .SetPosition(position)
                .SetRotation(rotation)
                .SetScale(scale);
        m_game_world->AddComponent(entity, transform_component);

        constexpr auto collider = Engine::Components::BoxCollider{
            .is_static = true,
            .is_trigger = true,
            .width = 0.5f,
            .height = 2.0f,
            .depth = 0.5f
        };
        m_game_world->AddComponent(entity, collider);
        m_game_world->AddComponent(entity, Components::Exit{});
    }


    void MazeBuilder::CreateMazeCell(const Cell& cell) const {
        const auto tile_material = DetermineFloorMaterialForCell(cell.cell_index);
        CreateCellFloorTile(cell.cell_index, tile_material);
        CreateCeilingTile(cell.cell_index);

        if (cell.HasWall(Front)) {
            CreateWallTile(cell.cell_index, Front);
        }
        if (cell.HasWall(Back)) {
            CreateWallTile(cell.cell_index, Back);
        }
        if (cell.HasWall(Left)) {
            CreateWallTile(cell.cell_index, Left);
        }
        if (cell.HasWall(Right)) {
            CreateWallTile(cell.cell_index, Right);
        }
    }

    void MazeBuilder::CreateCellFloorTile(
            const CellIndex& cell_idx,
            Engine::Assets::MaterialHandle material) const {
        const auto entity = m_game_world->CreateEntity(std::format("FloorTile [{}|{}]", cell_idx.x, cell_idx.y));
        const auto mesh_component = Engine::Components::MeshRenderer{
            .Mesh = m_floor_mesh,
            .Material = material,
        };
        m_game_world->AddComponent(entity, mesh_component);
        const auto position = glm::vec3(cell_idx.x * 2, 0.0f, cell_idx.y * 2);
        constexpr auto rotation = glm::vec3(0.0f, 0.0f, 0.0f);
        const auto transform_component = Engine::Components::Transform()
                .SetPosition(position)
                .SetRotation(rotation);
        m_game_world->AddComponent(entity, transform_component);
    }

    void MazeBuilder::GetShiftAndRotationVectorFromDirection(const Direction& direction, glm::vec3& shift_vector,
                                                             glm::vec3& rotation_shift) {
        shift_vector = glm::vec3(0.0f);
        rotation_shift = glm::vec3(0.0f);
        switch (direction) {
            case Back:
                shift_vector = glm::vec3(0.0f, 0.02f, -0.80f);
                rotation_shift = glm::vec3(0.0f, 0.0f, 0.0f);
                break;
            case Front:
                shift_vector = glm::vec3(0.0f, 0.02f, 0.8f);
                rotation_shift = glm::vec3(0.0f, 180.0f, 0.0f);
                break;
            case Left:
                shift_vector = glm::vec3(-0.8f, 0.02f, 0.0f);
                rotation_shift = glm::vec3(0.0f, 90.0f, 0.0f);
                break;
            case Right:
                shift_vector = glm::vec3(0.8f, 0.02f, 0.0f);
                rotation_shift = glm::vec3(0.0f, -90.0f, 0.0f);
                break;
        }
    }

    void MazeBuilder::CreateWallTile(const CellIndex& cell_idx,
                                     const Direction& direction) const {
        const auto entity = m_game_world->CreateEntity(
                std::format("WallTile [{}|{}]-{}", cell_idx.x, cell_idx.y, static_cast<int>(direction))
                );

        glm::vec3 shift_vector;
        glm::vec3 rotation_shift;
        GetShiftAndRotationVectorFromDirection(direction, shift_vector, rotation_shift);

        const auto mesh_component = Engine::Components::MeshRenderer{
            .Mesh = m_wall_mesh,
            .Material = m_wall_material,
        };

        m_game_world->AddComponent(entity, mesh_component);
        const auto position = glm::vec3(cell_idx.x * 2, 0.0f, cell_idx.y * 2) + shift_vector;
        const auto rotation = glm::vec3(0.0f, 0.0f, 0.0f) + rotation_shift;
        const auto scale = glm::vec3(0.5f, 0.5f, 0.5f);
        const auto transform_component = Engine::Components::Transform()
                .SetPosition(position)
                .SetRotation(rotation)
                .SetScale(scale);
        m_game_world->AddComponent(entity, transform_component);

        constexpr auto collider = Engine::Components::BoxCollider{
            .is_static = true,
            .width = 2.0f,
            .height = 2.0f,
            .depth = 1e-6f
        };
        m_game_world->AddComponent(entity, collider);
    }

    void MazeBuilder::CreateDoorTile(const CellIndex& cell_idx, const Direction& direction) const {
        glm::vec3 shift_vector;
        glm::vec3 rotation_shift;
        GetShiftAndRotationVectorFromDirection(direction, shift_vector, rotation_shift);
        const auto position = glm::vec3(cell_idx.x * 2, 0.0f, cell_idx.y * 2) + shift_vector;
        const auto rotation = glm::vec3(0.0f, 0.0f, 0.0f) + rotation_shift;
        const auto scale = glm::vec3(0.5f, 0.5f, 0.5f);


        const auto frame_entity = m_game_world->CreateEntity(
                std::format("DoorFrame [{}|{}]-{}", cell_idx.x, cell_idx.y, static_cast<int>(direction))
                );
        const auto frame_mesh_component = Engine::Components::MeshRenderer{
            .Mesh = m_door_frame,
            .Material = m_door_material,
        };
        const auto frame_transform_component = Engine::Components::Transform()
                .SetPosition(position)
                .SetRotation(rotation)
                .SetScale(scale);
        constexpr auto frame_door_trigger = Engine::Components::BoxCollider{
            .is_static = true,
            .is_trigger = true,
            .width = 2.0f,
            .height = 2.0f,
            .depth = 2.0f
        };
        m_game_world->AddComponent(frame_entity, frame_mesh_component);
        m_game_world->AddComponent(frame_entity, frame_transform_component);
        m_game_world->AddComponent(frame_entity, frame_door_trigger);


        const auto door_entity = m_game_world->CreateEntity(
                std::format("Door [{}|{}]-{}", cell_idx.x, cell_idx.y, static_cast<int>(direction))
                );
        const auto door_mesh_component = Engine::Components::MeshRenderer{
            .Mesh = m_door,
            .Material = m_door_material,
        };

        const auto door_transform_component = Engine::Components::Transform()
                .SetPosition(position)
                .SetRotation(rotation)
                .SetScale(scale);
        constexpr auto door_collider = Engine::Components::BoxCollider{
            .is_static = true,
            .width = 2.0f,
            .height = 2.0f,
            .depth = 1e-6f
        };
        m_game_world->AddComponent(door_entity, door_mesh_component);
        m_game_world->AddComponent(door_entity, door_transform_component);
        m_game_world->AddComponent(door_entity, door_collider);
    }


    void MazeBuilder::CreateCeilingTile(const CellIndex& cell_idx) const {
        const auto entity = m_game_world->CreateEntity(std::format("CeilingTile [{}|{}]", cell_idx.x, cell_idx.y));
        const auto mesh_component = Engine::Components::MeshRenderer{
            .Mesh = m_ceiling_mesh,
            .Material = m_ceiling_material,
        };
        m_game_world->AddComponent(entity, mesh_component);
        const auto position = glm::vec3(cell_idx.x * 2, 2.0f, cell_idx.y * 2);
        constexpr auto rotation = glm::vec3(180.0f, 0.0f, 0.0f);
        constexpr auto scale = glm::vec3(0.5f, 1.0f, 0.5f);
        const auto transform_component = Engine::Components::Transform()
                .SetPosition(position)
                .SetRotation(rotation)
                .SetScale(scale);
        m_game_world->AddComponent(entity, transform_component);
    }

    Engine::Assets::MaterialHandle MazeBuilder::DetermineFloorMaterialForCell(const CellIndex& cell_idx) const {
        auto material = m_default_material;
        if (cell_idx == m_maze.entrance_cell) {
            material = m_start_material;
        }
        if (cell_idx == m_maze.exit_cell) {
            material = m_exit_material;
        }
        return material;
    }
} // namespace
