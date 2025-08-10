//
// Created by Godri on 8/10/2025.
//

#include "RenderController.hpp"

#include "renderframework/OpenGL/OpenGLRenderer.hpp"


namespace Engine::Renderer {
    RenderController::RenderController(const Window::WindowContext &windowContext) {
        m_windowContext = windowContext;
        m_shaderManager = std::make_unique<ShaderManagement::ShaderManager>();
        m_shaderManager->LoadShader("test");

        switch (windowContext.renderApi) {
            case Window::API::OpenGL:
                m_renderer = std::make_unique<RenderFramework::OpenGL::OpenGLRenderer>(
                    m_windowContext, m_shaderManager.get());
                break;
            case Window::API::Vulkan:
                throw std::runtime_error("Vulkan renderer not supported (yet)");
                break;
            case Window::API::Metal:
                throw std::runtime_error("Metal renderer not supported (yet)");
                break;
        }
        m_renderer->Initialize();
    }

    RenderController::~RenderController() {
        m_renderer->Shutdown();
        m_renderer.release();
        m_shaderManager.release();
    }


    void RenderController::Render() {
        m_renderer->Render();
    }
}
