#include "EngineEventSystem.hpp"

#include <iostream>
#include <ostream>

namespace Engine::Core::Systems {
    void EngineEventSystem::Initialize() {
    }

    void EngineEventSystem::Run(float delta_time) {
        EcsWorld()->ApplyEngineEvents();
    }
} // namespace
