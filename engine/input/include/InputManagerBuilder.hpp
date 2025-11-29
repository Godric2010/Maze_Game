//
// Created by Sebastian Borsch on 29.11.25.
//

#pragma once
#include <memory>

#include "IInputManager.hpp"
#include "../src/InputManager.hpp"

namespace Engine::Input {
    static std::unique_ptr<IInputManager> CreateInputManager(Environment::IWindow* window) {
        auto input = std::make_unique<InputManager>(window);
        return input;
    }
}
