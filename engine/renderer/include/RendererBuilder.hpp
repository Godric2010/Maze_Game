#pragma once
#include <memory>

#include "Renderer.hpp"
#include "Window.hpp"
#include "../src/OpenGLRenderer.hpp"

namespace Engine::Renderer {
    /**
     * Create a new renderer instance based on the window context.
     * @param windowContext The window context that the rendere will use to display the results.
     * @return Return the renderer instance as a unique_ptr.
     */
    inline std::unique_ptr<IRenderer> CreateRenderer(Window::WindowContext windowContext) {
        switch (windowContext.renderApi) {
            case Window::API::OpenGL:
                return std::make_unique<OpenGLRenderer>(windowContext);
            default:
                return nullptr;
        }
    }
} // namespace
