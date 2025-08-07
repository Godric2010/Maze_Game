#pragma once
#include "Window.hpp"
#include "../include/Renderer.hpp"

namespace Engine::Renderer {
    class OpenGLRenderer final : public IRenderer {
    public:
        explicit OpenGLRenderer(Window::WindowContext windowContext);

        ~OpenGLRenderer() override;

        void Initialize() override;

        void Render() override;

        void Shutdown() override;

    private:
    };
} // namespace
