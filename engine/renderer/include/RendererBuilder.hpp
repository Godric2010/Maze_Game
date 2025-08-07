#pragma once
#include <memory>

#include "Renderer.hpp"
#include "Window.hpp"
#include "../src/OpenGLRenderer.hpp"

namespace Engine::Renderer {
    inline std::unique_ptr<IRenderer> CreateRenderer(Window::WindowContext windowContext) {
        switch (windowContext.renderApi) {
            case Window::API::OpenGL:
                return std::make_unique<OpenGLRenderer>(windowContext);
            default:
                return nullptr;
        }
    }
} // namespace
