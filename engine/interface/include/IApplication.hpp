//
// Created by Sebastian Borsch on 07.11.25.
//

#pragma once
#include "Scene/SceneArgs.hpp"
#include "Renderer/Types.hpp"
#include "Input/InputTypes.hpp"

namespace Engine {
    class IApplication {
    public:
        virtual ~IApplication() = default;

        virtual void Quit() = 0;

        virtual void RegisterScene(const std::string& name, SceneManagement::SceneFactory scene_factory) = 0;

        virtual void SetInitialScene(const std::string& name, const SceneManagement::SceneArgs& args) = 0;

        virtual Renderer::MeshHandle RegisterMesh(const Renderer::MeshAsset& mesh_asset) = 0;

        virtual void RegisterInputMap(Input::InputMap map) = 0;
    };
}
