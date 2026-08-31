//
// Created by Sebastian Borsch on 15.12.25.
//

#pragma once
#include "Ecs/ISystem.hpp"

namespace yarep::Ecs {
    struct EngineBindToken {
    private:
        EngineBindToken() = default;

        friend class SystemBinder;
    };
}
