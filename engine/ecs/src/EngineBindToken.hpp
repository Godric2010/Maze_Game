//
// Created by Sebastian Borsch on 15.12.25.
//

#pragma once
#include "Ecs/ISystem.hpp"

namespace Engine::Ecs {
    struct EngineBindToken {
    private:
        EngineBindToken() = default;

        friend class SystemBinder;
    };
}
