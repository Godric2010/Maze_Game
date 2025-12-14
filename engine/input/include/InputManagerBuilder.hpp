//
// Created by Sebastian Borsch on 29.11.25.
//

#pragma once
#include <memory>
#include "IInputManager.hpp"

namespace Engine::Input {
    class InputManagerBuilder {
    public:
        static std::unique_ptr<IInputManager> CreateInputManager(Environment::IWindow* window);
    };
}
