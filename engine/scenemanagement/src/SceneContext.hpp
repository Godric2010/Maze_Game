//
// Created by Sebastian Borsch on 05.11.25.
//

#pragma once
#include "IApplication.hpp"
#include "SystemManager.hpp"
#include "SceneWorld.hpp"
#include "Input/IInput.hpp"
#include "Renderer/IRenderer.hpp"
#include "Scene/ISceneManager.hpp"

namespace Engine::SceneManagement {
    struct SceneContext {
        IApplication& app;
        ISceneManager& scene_manager;
        Ecs::World& world;
        SceneWorld& game_world;
        Ecs::ISystemManager& system_manager;
        Input::IInput& input;
        Renderer::IRenderer& renderer;
        float screen_width;
        float screen_height;
    };
}
