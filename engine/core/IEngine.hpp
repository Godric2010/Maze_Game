//
// Created by Sebastian Borsch on 30.08.25.
//

#pragma once
#include "GameWorld.hpp"
#include "IScene.hpp"
#include "../renderer/Datatypes.hpp"
#include "SceneArgs.hpp"

namespace Engine::Core {
    class IEngine {
    public:
        virtual ~IEngine() = default;

        virtual void RegisterScene(const std::string &name, SceneFactory scene_factory) = 0;

        virtual void SetInitialScene(const std::string &name, const SceneArgs &args) = 0;

        virtual Renderer::MeshHandle RegisterMesh(const Renderer::MeshAsset &mesh_asset) = 0;

        virtual void RegisterInputMap(Input::InputMap map) = 0;
    };
}
