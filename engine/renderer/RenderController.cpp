//
// Created by Godri on 8/10/2025.
//

#include "RenderController.hpp"

#include "Renderable.hpp"
#include "renderframework/OpenGL/OpenGLRenderer.hpp"


namespace Engine::Renderer {
    RenderController::RenderController(const Window::WindowContext &windowContext) {
        m_windowContext = windowContext;
        m_camera = std::make_unique<Camera>(glm::vec3(0, 0, 3), windowContext.width, windowContext.height, 60, 0.01,
                                            100.0);
        m_shaderManager = std::make_unique<ShaderManagement::ShaderManager>();
        m_shaderManager->LoadShader("test");

        m_meshManager = std::make_unique<Meshmanagement::MeshManager>();
        auto quad_mesh = Meshmanagement::Mesh();
        quad_mesh.vertices = std::vector<glm::vec3>();
        quad_mesh.indices = std::vector<unsigned int>();
        quad_mesh.vertices.emplace_back(-0.5, -0.5, 0.0);
        quad_mesh.vertices.emplace_back(0.5, -0.5, 0.0);
        quad_mesh.vertices.emplace_back(0.5, 0.5, 0.0);
        quad_mesh.vertices.emplace_back(-0.5, 0.5, 0.0);
        quad_mesh.indices.push_back(0);
        quad_mesh.indices.push_back(1);
        quad_mesh.indices.push_back(2);
        quad_mesh.indices.push_back(2);
        quad_mesh.indices.push_back(3);
        quad_mesh.indices.push_back(0);
        m_meshManager->AddMesh(quad_mesh);

        m_renderables.emplace_back(glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), glm::vec3(1, 1, 1),
                                           glm::vec4(1, 0, 0, 1));


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
        m_renderer->AddMesh(quad_mesh);
    }

    RenderController::~RenderController() {
        m_renderer->Shutdown();
        m_renderer.reset();
        m_shaderManager.reset();
    }


    void RenderController::Render() const {
        m_renderer->PrepareFrame(m_camera->GetViewMatrix(), m_camera->GetProjectionMatrix(), m_camera->GetPosition());
        m_renderer->DrawFrame(m_renderables);
    }
}
