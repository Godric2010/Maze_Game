#pragma once
#include<format>
#include<string>
#include<cstdint>
#include "Camera.hpp"
#include "DebugGridDrawer.hpp"
#include "MazeAlgorithm.hpp"
#include "../MeshHandler.hpp"

namespace Gameplay::Mazegenerator {
    class MazeBuilder {
    public:
        MazeBuilder(Engine::Core::GameWorld *game_world, Engine::Renderer::MeshHandle floor_mesh,
                    Engine::Renderer::MeshHandle wall_mesh, Engine::Renderer::MeshHandle key_mesh,
                    bool enable_debug_view);

        ~MazeBuilder() = default;

        void BuildMaze(int width, int height, int seed);

        glm::vec3 GetMazeStartPosition() const;

    private:
        Engine::Core::GameWorld *m_game_world;
        std::unique_ptr<MazeAlgorithm> m_maze_algorithm;
        std::unique_ptr<DebugGridDrawer> m_debug_grid_drawer;
        Maze m_maze;
        Engine::Renderer::MeshHandle m_floor_mesh;
        Engine::Renderer::MeshHandle m_wall_mesh;
        Engine::Renderer::MeshHandle m_key_mesh;

        void CreateCellFloorTile(
            const CellIndex &cell_idx, const glm::vec4 &tile_color) const;

        void CreateWallFloorTile(const CellIndex &cell_idx,
                                 const glm::vec4 &tile_color, const Direction &direction) const;

        [[nodiscard]] glm::vec4 DetermineFloorColorForCell(const CellIndex &cell_idx) const;

        void CreateCellObjects() const;

        void CreateKeyObject(const CellIndex &cell_index) const;

        void CreateExitTrigger(const CellIndex &cell_index) const;

        void CreateMazeCell(const Cell &cell) const;
    };
} // namespace
