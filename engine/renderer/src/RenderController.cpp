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
                {
                    auto material_library = std::make_shared<RenderFramework::OpenGl::OpenGlMaterialLibrary>();
                    auto mesh_library = std::make_shared<RenderFramework::OpenGl::OpenGlMeshLibrary>();
                    auto texture_library = std::make_shared<RenderFramework::OpenGl::OpenGLTextureLibrary>();
                    auto shader_library = std::make_shared<RenderFramework::OpenGl::OpenGlShaderLibrary>(asset_handler);
                    m_renderer = std::make_unique<RenderFramework::OpenGl::OpenGlRenderer>(
                        m_window_context,
                        m_asset_handler,
                        material_library,
                        shader_library,
                        mesh_library,
                        texture_library
                    );
                    m_material_library = material_library;
                    m_shader_library = shader_library;
                    m_mesh_library = mesh_library;
                    m_texture_library = texture_library;
                    break;
                }
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

    void RenderController::RegisterMaterial(const AssetHandling::MaterialHandle& material_handle) const
    {
        auto material = m_asset_handler->GetAsset<AssetHandling::MaterialAsset>(material_handle);
        m_material_library->AddMaterial(material_handle, *material);
    }

    void RenderController::UnregisterMaterial(const Assets::MaterialHandle& material_handle) const
    {
        m_material_library->RemoveMaterial(material_handle);
    }

    void RenderController::SubmitDebugInfos(const std::vector<DrawAsset>& debug_draw_assets)
    {
        m_debug_draw_assets.clear();
        m_debug_draw_assets = debug_draw_assets;
    }

    void RenderController::RenderFrame(const CameraAsset& camera_asset, std::vector<DrawAsset> draw_assets) const
    {
        for (auto& debug_draw_asset : m_debug_draw_assets)
        {
            draw_assets.push_back(debug_draw_asset);
        }

        PrepareGpuResources(draw_assets);
        m_renderer->PrepareFrame(camera_asset);
        m_renderer->DrawFrame(draw_assets);
    }

    Assets::MeshHandle RenderController::GetUIMeshHandle() const
    {
        return m_ui_mesh_handle;
    }

    uint32_t RenderController::GetDrawCalls() const
    {
        return m_renderer->GetDrawCalls();
    }

    void RenderController::PrepareGpuResources(const std::vector<DrawAsset>& draw_assets) const
    {
        for (const auto& draw_asset : draw_assets)
        {
            if (!m_mesh_library->HasMesh(draw_asset.Mesh))
            {
                auto mesh_asset = m_asset_handler->GetAsset<AssetHandling::MeshAsset>(draw_asset.Mesh);
                m_mesh_library->AddMesh(draw_asset.Mesh, *mesh_asset);
            }

            if (!m_material_library->HasMaterial(draw_asset.Material))
            {
                auto material_asset = m_asset_handler->GetAsset<AssetHandling::MaterialAsset>(draw_asset.Material);
                m_material_library->AddMaterial(draw_asset.Material, *material_asset);

                auto albedo_texture_handle = material_asset->albedo_texture.texture;
                if (albedo_texture_handle && !m_texture_library->HasTexture(albedo_texture_handle))
                {
                    auto texture_asset = m_asset_handler->GetAsset<AssetHandling::TextureAsset>(albedo_texture_handle);
                    m_texture_library->AddTexture(albedo_texture_handle, *texture_asset);
                }
            }
        }
    }
}
