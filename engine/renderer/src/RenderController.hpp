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

namespace yarep::renderer
{
    class RenderController : public IRenderController
    {
    public:
        explicit RenderController(const environment::WindowContext& window_context,
                                  asset_handling::AssetHandler* asset_handler);

        ~RenderController() override;

        void SubmitDebugInfos(const std::vector<DrawAsset>& debug_draw_assets) override;
        
        void RenderFrame(const FrameData& frame_data, std::vector<DrawAsset> draw_assets) const override;

        assets::MeshHandle GetUiMeshHandle() const override;

        [[nodiscard]] uint32_t GetDrawCalls() const override;
        void PrepareMaterialsForGpu(const assets::MaterialHandle& handle) const;
        void PrepareMeshesForGpu(const assets::MeshHandle& mesh_handle) const;
        void PrepareTexturesForGpu(assets::TextureHandle handle) const;

    private:
        environment::WindowContext m_window_context;
        asset_handling::AssetHandler* m_asset_handler;
        std::unique_ptr<render_framework::IRenderer> m_renderer;
        std::vector<DrawAsset> m_debug_draw_assets;
        assets::MeshHandle m_ui_mesh_handle;
        
        std::shared_ptr<resources::IGpuMaterialLibrary> m_material_library;
        std::shared_ptr<resources::IGpuMeshLibrary> m_mesh_library;
        std::shared_ptr<resources::IGpuTextureLibrary> m_texture_library;
        std::shared_ptr<resources::IShaderLibrary> m_shader_library;
        
        void PrepareGpuResources(const std::vector<DrawAsset>& draw_assets) const;
    };
}
