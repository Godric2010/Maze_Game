#pragma once
#include <IApplication.hpp>
#include "MeshHandler.hpp"

namespace Gameplay {
    class GameplayManager {
    public:
        explicit GameplayManager(Engine::IApplication& engine);

        ~GameplayManager();

        void Initialize() const;

        void Shutdown();

    private:
        Engine::IApplication& m_engine;
        std::unique_ptr<MeshHandler> m_mesh_handler;
    };
} // namespace
