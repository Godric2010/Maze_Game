#include "EngineEventSystem.hpp"

namespace yarep::systems {
    void EngineEventSystem::Initialize() {
    }

    void EngineEventSystem::Run(float delta_time) {
        EcsWorld()->ApplyEngineEvents();
    }
} // namespace
