#pragma once
#include <IApplication.hpp>

namespace Gameplay {
    class GameplayManager {
    public:
        explicit GameplayManager(Engine::IApplication& engine);

        ~GameplayManager();

        void Initialize() const;

        void Shutdown();

    private:
        Engine::IApplication& m_engine;
    };
} // namespace
