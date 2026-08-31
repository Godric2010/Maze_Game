//
// Created by Sebastian Borsch on 24.08.25.
//

#pragma once
#include <vector>

#include "SystemManager.hpp"

namespace maze_game{
    std::vector<yarep::ecs::SystemMeta> GetSystemsFromGeneratedSource();
}
