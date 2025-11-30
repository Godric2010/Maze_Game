//
// Created by Sebastian Borsch on 05.11.25.
//

#pragma once
#include "GameWorld.hpp"
#include "IApplication.hpp"
#include "IInputManager.hpp"
#include "SystemManager.hpp"
#include "World.hpp"
#include "Scene/ISceneManager.hpp"

namespace Engine::Core {
    struct SceneContext {
        IApplication& app;
        ISceneManager& scene_manager;
        Ecs::World& world;
        GameWorld& game_world;
        Ecs::SystemManager& system_manager;
        Input::IInput& input;
        float screen_width;
        float screen_height;
    };
}
