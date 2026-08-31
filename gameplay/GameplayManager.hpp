#pragma once
#include <IApplication.hpp>

namespace gameplay {
    class GameplayManager {
    public:
        explicit GameplayManager(yarep::IApplication& engine);

        ~GameplayManager();

        void Initialize() const;

        void Shutdown();

    private:
        yarep::IApplication& m_engine;
    };
} // namespace
