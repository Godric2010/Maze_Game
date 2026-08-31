#include "EngineEventSystem.hpp"

#include <iostream>
#include <ostream>

namespace yarep::Systems {
    void EngineEventSystem::Initialize() {
    }

    void EngineEventSystem::Run(float delta_time) {
        EcsWorld()->ApplyEngineEvents();
    }
} // namespace
