#pragma once
#include <memory>

#include "AssetHandler.hpp"
#include "Window.hpp"
#include "Datatypes.hpp"
#include "IRenderController.hpp"
#include "resources/IGpuMeshLibrary.hpp"
#include "resources/IGpuTextureLibrary.hpp"
#include "resources/IShaderLibrary.hpp"
#include "renderframework/Renderer.hpp"
#include "resources/IGpuMaterialLibrary.hpp"

namespace Engine::Renderer
{
    class RenderController : public IRenderController
    {
    public:
        explicit RenderController(const Environment::WindowContext& window_context,
                                  AssetHandling::AssetHandler* asset_handler);

        ~RenderController() override;

        void RegisterMesh(const AssetHandling::MeshAsset& mesh, const Assets::MeshHandle& handle) const override;

        void UnregisterMesh(const Assets::MeshHandle& handle) const override;

        void RegisterTexture(const AssetHandling::TextureAsset& texture,
                             const Assets::TextureHandle& handle) const override;

        void UnregisterTexture(const Assets::TextureHandle& handle) const override;

        void RegisterMaterial(const AssetHandling::MaterialHandle& material_handle) const override;

        void UnregisterMaterial(const AssetHandling::MaterialHandle& material_handle) const override;

        void SubmitDebugInfos(const std::vector<DrawAsset>& debug_draw_assets) override;
        
        void RenderFrame(const CameraAsset& camera_asset, std::vector<DrawAsset> draw_assets) const override;

        Assets::MeshHandle GetUIMeshHandle() const override;

        [[nodiscard]] uint32_t GetDrawCalls() const override;

    private:
        Environment::WindowContext m_window_context;
        AssetHandling::AssetHandler* m_asset_handler;
        std::unique_ptr<RenderFramework::IRenderer> m_renderer;
        std::vector<DrawAsset> m_debug_draw_assets;
        Assets::MeshHandle m_ui_mesh_handle;
        
        std::shared_ptr<Resources::IGpuMaterialLibrary> m_material_library;
        std::shared_ptr<Resources::IGpuMeshLibrary> m_mesh_library;
        std::shared_ptr<Resources::IGpuTextureLibrary> m_texture_library;
        std::shared_ptr<Resources::IShaderLibrary> m_shader_library;
        
        void PrepareGpuResources(const std::vector<DrawAsset>& draw_assets) const;
    };
}
