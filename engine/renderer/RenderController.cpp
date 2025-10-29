//
// Created by Godri on 8/10/2025.
//

#include "RenderController.hpp"

#include "renderframework/OpenGL/OpenGLRenderer.hpp"


namespace Engine::Renderer {
    RenderController::RenderController(const Environment::WindowContext &window_context) {
        m_window_context = window_context;

        m_shader_manager = std::make_unique<ShaderManagement::ShaderManager>();
        m_shader_manager->LoadShader("mesh_opaque");
        m_shader_manager->LoadShader("ui");

        switch (window_context.renderApi) {
            case Environment::API::OpenGL:
                m_renderer = std::make_unique<RenderFramework::OpenGl::OpenGlRenderer>(
                    m_window_context, m_shader_manager.get());
                break;
            case Environment::API::Vulkan:
                throw std::runtime_error("Vulkan renderer not supported (yet)");
                break;
            case Environment::API::Metal:
                throw std::runtime_error("Metal renderer not supported (yet)");
                break;
        }
        m_renderer->Initialize();
        m_renderer->AddMesh(CreateUiPrimitive());
    }

    RenderController::~RenderController() {
        m_renderer->Shutdown();
        m_renderer.reset();
        m_shader_manager.reset();
    }

    MeshHandle RenderController::RegisterMesh(const MeshAsset &mesh) const {
        return m_renderer->AddMesh(mesh);
    }

    void RenderController::UnregisterMesh(const MeshHandle &handle) const {
        m_renderer->RemoveMesh(handle);
    }

    void RenderController::BeginFrame(const CameraAsset &camera_asset) const {
        m_renderer->PrepareFrame(camera_asset);
    }

    void RenderController::SubmitFrame(DrawAssets &draw_assets) const {
        m_renderer->DrawFrame(draw_assets);
    }
}
