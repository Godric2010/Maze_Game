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

        auto material_library = std::make_unique<RenderFramework::Materials::MaterialLibrary>(asset_handler);
        switch (window_context.renderApi)
        {
            case Environment::API::OpenGL:
                m_renderer = std::make_unique<RenderFramework::OpenGl::OpenGlRenderer>(
                    m_window_context,
                    m_asset_handler,
                    std::move(material_library)
                );
                break;
            case Environment::API::Vulkan:
                throw std::runtime_error("Vulkan renderer not supported (yet)");
            case Environment::API::Metal:
                throw std::runtime_error("Metal renderer not supported (yet)");
        }
        m_renderer->Initialize();

        m_ui_mesh_handle = m_asset_handler->RegisterAsset(CreateUiPrimitive());
        const auto asset = m_asset_handler->GetAsset<AssetHandling::MeshAsset>(m_ui_mesh_handle);
        m_renderer->AddMesh(*asset, m_ui_mesh_handle);
    }

    RenderController::~RenderController()
    {
        m_asset_handler = nullptr;
        m_renderer->Shutdown();
        m_renderer.reset();
    }

    Assets::MeshHandle RenderController::GetOrLoadMesh(const std::string& file_path)
    {
        const auto mesh_handle = m_asset_handler->LoadAsset<AssetHandling::MeshAsset>(file_path);
        const auto mesh_asset = m_asset_handler->GetAsset<AssetHandling::MeshAsset>(mesh_handle);
        m_renderer->AddMesh(*mesh_asset, mesh_handle);
        return mesh_handle;
    }

    Assets::TextureHandle RenderController::GetOrLoadTexture(const std::string& file_path)
    {
        const auto texture_handle = m_asset_handler->LoadAsset<AssetHandling::TextureAsset>(file_path);
        const auto texture_asset = m_asset_handler->GetAsset<AssetHandling::TextureAsset>(texture_handle);
        RegisterTexture(*texture_asset, texture_handle);
        return texture_handle;
    }

    Assets::MaterialHandle RenderController::GetOrLoadMaterial(const std::string& file_path)
    {
        const auto material_handle = m_asset_handler->LoadAsset<AssetHandling::MaterialAsset>(file_path);
        m_renderer->GetMaterialLibrary()->Add(material_handle);
        auto material = m_asset_handler->GetAsset<AssetHandling::MaterialAsset>(material_handle);
        const auto albedo_texture_handle = material->albedo_texture.texture;
        if (albedo_texture_handle)
        {
            const auto albedo_texture = m_asset_handler->GetAsset<AssetHandling::TextureAsset>(albedo_texture_handle);
            RegisterTexture(*albedo_texture, albedo_texture_handle);
        }
        return material_handle;
    }

    void RenderController::RegisterMesh(const AssetHandling::MeshAsset& mesh, const Assets::MeshHandle& handle) const
    {
        return m_renderer->AddMesh(mesh, handle);
    }

    void RenderController::UnregisterMesh(const Assets::MeshHandle& handle) const
    {
        m_renderer->RemoveMesh(handle);
    }

    void RenderController::RegisterTexture(const AssetHandling::TextureAsset& texture,
                                           const Assets::TextureHandle& handle) const
    {
        return m_renderer->AddTexture(texture, handle);
    }

    void RenderController::UnregisterTexture(const Assets::TextureHandle& handle) const
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

    Assets::MeshHandle RenderController::GetUIMeshHandle() const
    {
        return m_ui_mesh_handle;
    }

    uint32_t RenderController::GetDrawCalls() const
    {
        return m_renderer->GetDrawCalls();
    }
}
