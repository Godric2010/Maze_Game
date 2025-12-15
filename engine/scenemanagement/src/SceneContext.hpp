//
// Created by Sebastian Borsch on 05.11.25.
//

#pragma once
#include "IApplication.hpp"
#include "SystemManager.hpp"
#include "SceneWorld.hpp"
#include "Input/IInput.hpp"
#include "Scene/ISceneManager.hpp"

namespace Engine::SceneManagement {
    struct SceneContext {
        IApplication& app;
        ISceneManager& scene_manager;
        Ecs::World& world;
        SceneWorld& game_world;
        Ecs::SystemManager& system_manager;
        Input::IInput& input;
        float screen_width;
        float screen_height;
    };
}
