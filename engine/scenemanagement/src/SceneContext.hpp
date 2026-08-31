//
// Created by Sebastian Borsch on 05.11.25.
//

#pragma once
#include "IApplication.hpp"
#include "SystemManager.hpp"
#include "SceneWorld.hpp"
#include "Input/IInput.hpp"
#include "Scene/ISceneManager.hpp"

namespace yarep::scene_management {
    struct SceneContext {
        IApplication& app;
        ISceneManager& scene_manager;
        assets::IAssetLibrary& asset_library;
        ecs::World& world;
        SceneWorld& game_world;
        ecs::ISystemManager& system_manager;
        input::IInput& input;
        float screen_width;
        float screen_height;
    };
}
