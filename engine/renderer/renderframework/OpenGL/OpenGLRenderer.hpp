#pragma once
#include "Window.hpp"
#include "../../include/Renderer.hpp"
#include <glad/glad.h>
#include <array>

namespace Engine::Renderer {
    class OpenGLRenderer final : public IRenderer {
    public:
        explicit OpenGLRenderer(Window::WindowContext windowContext);

        ~OpenGLRenderer() override;

        void Initialize() override;

        void Render() override;

        void Shutdown() override;

    private:
        unsigned int m_VAO;
        unsigned int m_VBO;
        unsigned int m_shaderProgram;

        static unsigned int LoadShaders();
    };
} // namespace
