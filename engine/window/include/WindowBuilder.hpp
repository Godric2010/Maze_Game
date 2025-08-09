#pragma once
#include <memory>

#include "Window.hpp"
#include "../src/SDLWindow.hpp"

namespace Engine::Window {
    /**
     * Create a new window instance.
     * @return The new window instance as unique_ptr.
     */
    static std::unique_ptr<IWindow> CreateWindow() {
        return std::make_unique<SDLWindow>();
    }
} // namespace