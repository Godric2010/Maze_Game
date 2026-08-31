#pragma once
#include "DebugGridDrawer.hpp"
#include "MazeAlgorithm.hpp"
#include "SceneWorld.hpp"
#include "Assets/AssetHandleTypes.hpp"
#include <glm/glm.hpp>

#include "Assets/IAssetLibrary.hpp"

namespace gameplay::maze_generator {
    class MazeBuilder {
    public:
        MazeBuilder(yarep::SceneManagement::SceneWorld* game_world, yarep::Assets::IAssetLibrary* renderer,
                    bool enable_debug_view);

        ~MazeBuilder() = default;

        void BuildMaze(int width, int height, int seed);

        glm::vec3 GetMazeStartPosition() const;

    private:
        yarep::SceneManagement::SceneWorld* m_game_world;
        yarep::Assets::IAssetLibrary* m_assets;
        std::unique_ptr<MazeAlgorithm> m_maze_algorithm;
        std::unique_ptr<DebugGridDrawer> m_debug_grid_drawer;
        Maze m_maze;
        yarep::Assets::MeshHandle m_floor_mesh;
        yarep::Assets::MeshHandle m_wall_mesh;
        yarep::Assets::MeshHandle m_ceiling_mesh;
        yarep::Assets::MeshHandle m_key_mesh;
        yarep::Assets::MeshHandle m_door_frame;
        yarep::Assets::MeshHandle m_door;
        yarep::Assets::MaterialHandle m_default_material;
        yarep::Assets::MaterialHandle m_key_material;
        yarep::Assets::MaterialHandle m_start_material;
        yarep::Assets::MaterialHandle m_exit_material;
        yarep::Assets::MaterialHandle m_wall_material;
        yarep::Assets::MaterialHandle m_ceiling_material;
        yarep::Assets::MaterialHandle m_door_material;

        void CreateCellFloorTile(const CellIndex& cell_idx, yarep::Assets::MaterialHandle material) const;

        void CreateWallTile(const CellIndex& cell_idx, const Direction& direction) const;

        void CreateDoorTile(const CellIndex& cell_idx, const Direction& direction) const;

        void CreateCeilingTile(const CellIndex& cell_idx) const;

        [[nodiscard]] yarep::Assets::MaterialHandle DetermineFloorMaterialForCell(const CellIndex& cell_idx) const;

        void CreateCellObjects() const;

        void CreateExitCell(const Cell& exit_cell) const;

        void CreateKeyObject(const CellIndex& cell_index) const;

        void CreateExitTrigger(const CellIndex& cell_index) const;

        void CreateCeilingLight(CellIndex cell_index) const;

        void CreateMazeCell(const Cell& cell) const;

        static void GetShiftAndRotationVectorFromDirection(const Direction& direction, glm::vec3& shift_vector,
                                                           glm::vec3& rotation_shift);
    };
} // namespace
