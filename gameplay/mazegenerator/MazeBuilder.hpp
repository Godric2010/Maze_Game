#pragma once
#include "DebugGridDrawer.hpp"
#include "MazeAlgorithm.hpp"
#include "SceneWorld.hpp"
#include "Assets/AssetHandleTypes.hpp"
#include <glm/glm.hpp>

namespace Gameplay::Mazegenerator
{
    class MazeBuilder
    {
    public:
        MazeBuilder(Engine::SceneManagement::SceneWorld* game_world, Engine::Assets::MeshHandle floor_mesh,
                    Engine::Assets::MeshHandle wall_mesh, Engine::Assets::MeshHandle key_mesh,
                    Engine::Assets::TextureHandle texture,
                    Engine::Assets::MaterialHandle material,
                    bool enable_debug_view);

        ~MazeBuilder() = default;

        void BuildMaze(int width, int height, int seed);

        glm::vec3 GetMazeStartPosition() const;

    private:
        Engine::SceneManagement::SceneWorld* m_game_world;
        std::unique_ptr<MazeAlgorithm> m_maze_algorithm;
        std::unique_ptr<DebugGridDrawer> m_debug_grid_drawer;
        Maze m_maze;
        Engine::Assets::MeshHandle m_floor_mesh;
        Engine::Assets::MeshHandle m_wall_mesh;
        Engine::Assets::MeshHandle m_key_mesh;
        Engine::Assets::TextureHandle m_texture;
        Engine::Assets::MaterialHandle m_material;

        void CreateCellFloorTile(
            const CellIndex& cell_idx, const glm::vec4& tile_color) const;

        void CreateWallFloorTile(const CellIndex& cell_idx,
                                 const glm::vec4& tile_color, const Direction& direction) const;

        [[nodiscard]] glm::vec4 DetermineFloorColorForCell(const CellIndex& cell_idx) const;

        void CreateCellObjects() const;

        void CreateKeyObject(const CellIndex& cell_index) const;

        void CreateExitTrigger(const CellIndex& cell_index) const;

        void CreateMazeCell(const Cell& cell) const;
    };
} // namespace
