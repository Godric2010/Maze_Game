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


        m_asset_handler->LoadShader("mesh_opaque");
        m_asset_handler->LoadShader("ui");

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
    }

    RenderController::~RenderController()
    {
        m_asset_handler = nullptr;
        m_renderer->Shutdown();
        m_renderer.reset();
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
            PrepareMeshesForGpu(draw_asset.Mesh);
            PrepareMaterialsForGpu(draw_asset.Material);
        }
    }

    void RenderController::PrepareMeshesForGpu(const Assets::MeshHandle& mesh_handle) const
    {
        const auto mesh_revision = m_asset_handler->GetAssetRevision<AssetHandling::MeshAsset>(mesh_handle);
        if (!m_mesh_library->HasMesh(mesh_handle))
        {
            const auto mesh_asset = m_asset_handler->GetAsset<AssetHandling::MeshAsset>(mesh_handle);
            m_mesh_library->AddMesh(mesh_handle, *mesh_asset, mesh_revision);
        }
        else if (m_mesh_library->GetMeshRevision(mesh_handle) != mesh_revision)
        {
            const auto mesh_asset = m_asset_handler->GetAsset<AssetHandling::MeshAsset>(mesh_handle);
            m_mesh_library->RemoveMesh(mesh_handle);
            m_mesh_library->AddMesh(mesh_handle, *mesh_asset, mesh_revision);
        }
    }


    void RenderController::PrepareMaterialsForGpu(const Assets::MaterialHandle& handle) const
    {
        const auto material_revision = m_asset_handler->GetAssetRevision<AssetHandling::MaterialAsset>(handle);
        const auto material_asset = m_asset_handler->GetAsset<AssetHandling::MaterialAsset>(handle);
        if (!m_material_library->HasMaterial(handle))
        {
            m_material_library->AddMaterial(handle, *material_asset, material_revision);
        }
        else if (m_material_library->GetMaterialRevision(handle) != material_revision)
        {
            m_material_library->RemoveMaterial(handle);
            m_material_library->AddMaterial(handle, *material_asset, material_revision);
        }

        const auto albedo_texture_handle = material_asset->albedo_texture.texture;
        if (!albedo_texture_handle)
        {
            return;
        }

        PrepareTexturesForGpu(albedo_texture_handle);
    }

    void RenderController::PrepareTexturesForGpu(const Assets::TextureHandle handle) const
    {
        const auto texture_revision = m_asset_handler->GetAssetRevision<AssetHandling::TextureAsset>(handle);
        if (!m_texture_library->HasTexture(handle))
        {
            const auto texture_asset = m_asset_handler->GetAsset<AssetHandling::TextureAsset>(handle);
            m_texture_library->AddTexture(handle, *texture_asset, texture_revision);
        }
        else if (m_texture_library->GetTextureRevision(handle) != texture_revision)
        {
            m_texture_library->RemoveTexture(handle);
            const auto texture_asset = m_asset_handler->GetAsset<AssetHandling::TextureAsset>(handle);
            m_texture_library->AddTexture(handle, *texture_asset, texture_revision);
        }
    }
}
