//
// Created by Sebastian Borsch on 24.08.25.
//

#pragma once
#include <vector>

#include "SystemManager.hpp"

namespace MazeGame{
    std::vector<Engine::Ecs::SystemMeta> GetSystemsFromGeneratedSource();
}
