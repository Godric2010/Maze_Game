#include "EngineEventSystem.hpp"

#include <iostream>
#include <ostream>

namespace yarep::systems {
    void EngineEventSystem::Initialize() {
    }

    void EngineEventSystem::Run(float delta_time) {
        EcsWorld()->ApplyEngineEvents();
    }
} // namespace
