#pragma once
#include <EngineController.hpp>

#include "Camera.hpp"
#include "Camera.hpp"
#include "Mesh.hpp"
#include "MeshHandler.hpp"
#include "mazegenerator/MazeAlgorithm.hpp"
#include "mazegenerator/MazeBuilder.hpp"

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
        std::unique_ptr<Mazegenerator::MazeBuilder> m_maze_builder;
        std::unique_ptr<MeshHandler> m_mesh_handler;

        void CreateCamera(glm::vec3 start_pos);

        static glm::vec3 ConvertDirection(const Mazegenerator::Direction &direction);
    };
} // namespace
