#pragma once
#include "Window.hpp"
#include "../Renderer.hpp"
#include <glad/glad.h>
#include <array>

#include "../../shadermanagement/ShaderManager.hpp"

namespace Engine::Renderer::RenderFramework::OpenGL {
    class OpenGLRenderer final : public IRenderer {
    public:
        explicit OpenGLRenderer(Window::WindowContext windowContext, ShaderManagement::ShaderManager* shaderManager);

        ~OpenGLRenderer() override;

        void Initialize() override;

        void Render() override;

        void Shutdown() override;

    private:
        unsigned int m_VAO;
        unsigned int m_VBO;
        unsigned int m_shaderProgram;

        ShaderManagement::ShaderManager* m_shaderManager;
        unsigned int LoadShaders() const;
    };
} // namespace
