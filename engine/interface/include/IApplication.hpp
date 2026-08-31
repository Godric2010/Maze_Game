//
// Created by Sebastian Borsch on 07.11.25.
//

#pragma once
#include "Scene/SceneArgs.hpp"

namespace yarep {
    class IApplication {
    public:
        virtual ~IApplication() = default;

        virtual void Quit() = 0;

        virtual void RegisterScene(const std::string& name, scene_management::SceneFactory scene_factory) = 0;

        virtual void SetInitialScene(const std::string& name, const scene_management::SceneArgs& args) = 0;
    };
}
