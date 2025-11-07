//
// Created by Sebastian Borsch on 05.11.25.
//

#pragma once
#include "GameWorld.hpp"
#include "SystemManager.hpp"
#include "World.hpp"
#include "InputManager.hpp"

namespace Engine::Core {
    struct SceneContext {
        Ecs::World &world;
        GameWorld &game_world;
        Ecs::SystemManager &system_manager;
        InputManager &input;
        float screen_width;
        float screen_height;
    };
}
