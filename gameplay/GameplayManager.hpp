#pragma once
#include <EngineController.hpp>

#include "Mesh.hpp"
#include "MeshHandler.hpp"
#include "mazegenerator/MazeAlgorithm.hpp"

namespace Gameplay {
    class GameplayManager {
    public:
        explicit GameplayManager(Engine::Core::IEngine &engine);

        ~GameplayManager();

        void Initialize();

        void Shutdown();

    private:
        Engine::Core::IEngine &m_engine;

        Engine::Ecs::EntityId m_camera_entity;
        std::vector<Engine::Ecs::EntityId> m_objects;
        std::unique_ptr<Mazegenerator::MazeAlgorithm> m_maze_algorithm;
        std::unique_ptr<MeshHandler> m_mesh_handler;

        void CreateCamera(const Mazegenerator::CellIndex &start_pos);

        void CreateCellFloorTile(Engine::Renderer::MeshHandle mesh_handle,
                                 const Mazegenerator::CellIndex &cell_idx, const glm::vec4 &tile_color) const;

        void CreateWallFloorTile(Engine::Renderer::MeshHandle mesh_handle, const Mazegenerator::CellIndex &cell_idx,
                                 const glm::vec4 &tile_color, const Mazegenerator::Direction &direction) const;

        static glm::vec4 DetermineFloorColorForCell(const Mazegenerator::Maze &maze,
                                                    const Mazegenerator::CellIndex &cell_idx);

        void CreateObjects(const Mazegenerator::Maze &maze) const;

        void CreateKeyObject(const Mazegenerator::CellIndex &cell_index,
                             const Engine::Renderer::MeshHandle &key_mesh_handle) const;

        void CreateMazeCell(const Mazegenerator::Maze &maze, const Mazegenerator::Cell &cell,
                            const Engine::Renderer::MeshHandle &floor_mesh_handle,
                            const Engine::Renderer::MeshHandle &wall_mesh_handle) const;

        static glm::vec3 ConvertDirection(const Mazegenerator::Direction &direction);
    };
} // namespace
