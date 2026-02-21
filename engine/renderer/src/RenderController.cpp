//
// Created by Godri on 8/10/2025.
//

#include <spdlog/spdlog.h>
#include "RenderController.hpp"

#include "PrimitiveMeshes.hpp"
#include "renderframework/OpenGL/OpenGLRenderer.hpp"


namespace Engine::Renderer
{
    RenderController::RenderController(const Environment::WindowContext& window_context,
                                       AssetHandling::AssetHandler* asset_handler)
    {
        m_window_context = window_context;
        m_asset_handler = asset_handler;

        m_asset_handler->LoadAsset<AssetHandling::ShaderAsset>("mesh_opaque");
        m_asset_handler->LoadAsset<AssetHandling::ShaderAsset>("ui");

        switch (window_context.renderApi)
        {
            case Environment::API::OpenGL:
                m_renderer = std::make_unique<RenderFramework::OpenGl::OpenGlRenderer>(
                    m_window_context,
                    m_asset_handler
                );
                break;
            case Environment::API::Vulkan:
                throw std::runtime_error("Vulkan renderer not supported (yet)");
            case Environment::API::Metal:
                throw std::runtime_error("Metal renderer not supported (yet)");
        }
        m_renderer->Initialize();
        m_renderer->AddMesh(CreateUiPrimitive());
    }

    RenderController::~RenderController()
    {
        m_asset_handler = nullptr;
        m_renderer->Shutdown();
        m_renderer.reset();
    }

    MeshHandle RenderController::GetOrLoadMesh(const std::string& file_path)
    {
        const auto mesh_asset = m_asset_handler->LoadAsset<AssetHandling::MeshAsset>(file_path);
        const auto mesh_handle = m_renderer->AddMesh(*mesh_asset);
        return mesh_handle;
    }

    TextureHandle RenderController::GetOrLoadTexture(const std::string& file_path)
    {
        const auto texture_asset = m_asset_handler->LoadAsset<AssetHandling::TextureAsset>(file_path);
        const auto texture_handle = RegisterTexture(*texture_asset);
        return texture_handle;
    }

    MaterialHandle RenderController::GetOrLoadMaterial(const std::string& file_path)
    {
        auto material_asset = m_asset_handler->LoadAsset<AssetHandling::MaterialAsset>(file_path);

        return 0;
    }

    MeshHandle RenderController::RegisterMesh(const AssetHandling::MeshAsset& mesh) const
    {
        return m_renderer->AddMesh(mesh);
    }

    void RenderController::UnregisterMesh(const MeshHandle& handle) const
    {
        m_renderer->RemoveMesh(handle);
    }

    TextureHandle RenderController::RegisterTexture(const AssetHandling::TextureAsset& texture) const
    {
        return m_renderer->AddTexture(texture);
    }

    void RenderController::UnregisterTexture(const TextureHandle& handle) const
    {
        m_renderer->RemoveTexture(handle);
    }

    void RenderController::BeginFrame(const CameraAsset& camera_asset) const
    {
        m_renderer->PrepareFrame(camera_asset);
    }

    void RenderController::SubmitFrame(DrawAssets& draw_assets) const
    {
        for (auto& debug_draw_asset : m_debug_draw_assets)
        {
            draw_assets.ui_draw_assets.push_back(debug_draw_asset);
        }
        m_renderer->DrawFrame(draw_assets);
    }

    void RenderController::SubmitDebugInfos(const std::vector<UiDrawAsset>& debug_draw_assets)
    {
        m_debug_draw_assets.clear();
        m_debug_draw_assets = debug_draw_assets;
    }

    uint32_t RenderController::GetDrawCalls() const
    {
        return m_renderer->GetDrawCalls();
    }
}
