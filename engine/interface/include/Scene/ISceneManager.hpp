//
// Created by Sebastian Borsch on 08.11.25.
//

#pragma once
#include <string>

#include "SceneArgs.hpp"

namespace Engine::SceneManagement {
    class ISceneManager {
    public:
        virtual ~ISceneManager() = default;

        virtual void LoadScene(const std::string &name, const SceneArgs &args) = 0;
    };
}
