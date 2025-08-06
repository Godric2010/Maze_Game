#pragma once
#include <memory>

#include "Window.hpp"
#include "../src/SDLWindow.hpp"

namespace Engine::Window {

    static std::unique_ptr<IWindow> CreateWindow() {
        return std::make_unique<SDLWindow>();
    }
} // namespace