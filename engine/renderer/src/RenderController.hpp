#pragma once
#include <memory>

#include "AssetHandler.hpp"
#include "Window.hpp"
#include "../include/Datatypes.hpp"
#include "../include/IRenderController.hpp"
#include "renderframework/Renderer.hpp"

namespace Engine::Renderer
{
    class RenderController : public IRenderController
    {
    public:
        explicit RenderController(const Environment::WindowContext& window_context,
                                  AssetHandling::AssetHandler* asset_handler);

        ~RenderController() override;

        Assets::MeshHandle GetOrLoadMesh(const std::string& file_path) override;

        Assets::TextureHandle GetOrLoadTexture(const std::string& file_path) override;

        Assets::MaterialHandle GetOrLoadMaterial(const std::string& file_path) override;

        void RegisterMesh(const AssetHandling::MeshAsset& mesh, const Assets::MeshHandle& handle) const override;

        void UnregisterMesh(const Assets::MeshHandle& handle) const override;

        void RegisterTexture(const AssetHandling::TextureAsset& texture,
                             const Assets::TextureHandle& handle) const override;

        void UnregisterTexture(const Assets::TextureHandle& handle) const override;

        void BeginFrame(const CameraAsset& camera_asset) const override;

        void SubmitFrame(DrawAssets& draw_assets) const override;

        void SubmitDebugInfos(const std::vector<UiDrawAsset>& debug_draw_assets) override;
        
        Assets::MeshHandle GetUIMeshHandle() const override;

        [[nodiscard]] uint32_t GetDrawCalls() const override;

    private:
        Environment::WindowContext m_window_context;
        AssetHandling::AssetHandler* m_asset_handler;
        std::unique_ptr<RenderFramework::IRenderer> m_renderer;
        std::vector<UiDrawAsset> m_debug_draw_assets;
        Assets::MeshHandle m_ui_mesh_handle;
    };
}
