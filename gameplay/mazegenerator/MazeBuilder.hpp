#pragma once
#include "DebugGridDrawer.hpp"
#include "MazeAlgorithm.hpp"
#include "SceneWorld.hpp"
#include "Assets/AssetHandleTypes.hpp"
#include <glm/glm.hpp>

#include "Assets/IAssetLibrary.hpp"
#include "Renderer/IRenderer.hpp"

namespace Gameplay::Mazegenerator
{
    class MazeBuilder
    {
    public:
        MazeBuilder(Engine::SceneManagement::SceneWorld* game_world, Engine::Assets::IAssetLibrary* renderer,
                    bool enable_debug_view);

        ~MazeBuilder() = default;

        void BuildMaze(int width, int height, int seed);

        glm::vec3 GetMazeStartPosition() const;

    private:
        Engine::SceneManagement::SceneWorld* m_game_world;
        Engine::Assets::IAssetLibrary* m_assets;
        std::unique_ptr<MazeAlgorithm> m_maze_algorithm;
        std::unique_ptr<DebugGridDrawer> m_debug_grid_drawer;
        Maze m_maze;
        Engine::Assets::MeshHandle m_floor_mesh;
        Engine::Assets::MeshHandle m_wall_mesh;
        Engine::Assets::MeshHandle m_key_mesh;
        Engine::Assets::MaterialHandle m_default_material;
        Engine::Assets::MaterialHandle m_key_material;
        Engine::Assets::MaterialHandle m_start_material;
        Engine::Assets::MaterialHandle m_exit_material;
        Engine::Assets::MaterialHandle m_wall_material;

        void CreateCellFloorTile(
            const CellIndex& cell_idx, Engine::Assets::MaterialHandle material) const;

        void CreateWallTile(const CellIndex& cell_idx,
                                 const Direction& direction) const;

        [[nodiscard]] Engine::Assets::MaterialHandle DetermineFloorMaterialForCell(const CellIndex& cell_idx) const;

        void CreateCellObjects() const;

        void CreateKeyObject(const CellIndex& cell_index) const;

        void CreateExitTrigger(const CellIndex& cell_index) const;

        void CreateMazeCell(const Cell& cell) const;
    };
} // namespace
