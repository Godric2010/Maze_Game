#pragma once
#include <EngineController.hpp>

#include "MeshHandler.hpp"

namespace Gameplay {
    class GameplayManager {
    public:
        explicit GameplayManager(Engine::Core::IEngine &engine);

        ~GameplayManager();

        void Initialize();

        void Shutdown();

    private:
        Engine::Core::IEngine &m_engine;
        std::unique_ptr<MeshHandler> m_mesh_handler;
    };
} // namespace
