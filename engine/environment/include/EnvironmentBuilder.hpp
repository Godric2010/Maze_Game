#pragma once
#include <memory>

#include "Input.hpp"
#include "Window.hpp"
#include "../src/SDLInput.hpp"
#include "../src/SDLWindow.hpp"

namespace Engine::Environment {
    /**
     * Create a new window instance.
     * @return The new window instance as unique_ptr.
     */
    static std::unique_ptr<IWindow> CreateWindow() {
        return std::make_unique<SDLWindow>();
    }

    static std::unique_ptr<IInput> CreateInput(IWindow &window) {
        return std::make_unique<SDLInput>(reinterpret_cast<SDLWindow &>(window));
    }
} // namespace
