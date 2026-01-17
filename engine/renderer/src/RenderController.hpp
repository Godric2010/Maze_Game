#pragma once
#include <memory>

#include "Window.hpp"
#include "../include/Datatypes.hpp"
#include "../include/IRenderController.hpp"
#include "renderframework/Renderer.hpp"
#include "shadermanagement/ShaderManager.hpp"

namespace Engine::Renderer {
    class RenderController : public IRenderController {
    public:
        explicit RenderController(const Environment::WindowContext& window_context, Environment::Files::IFileReader* file_reader);

        ~RenderController() override;

        [[nodiscard]] MeshHandle RegisterMesh(const MeshAsset& mesh) const override;

        void UnregisterMesh(const MeshHandle& handle) const override;

        [[nodiscard]] TextureHandle RegisterTexture(const TextureAsset& texture) const override;

        void UnregisterTexture(const TextureHandle& handle) const override;

        void BeginFrame(const CameraAsset& camera_asset) const override;

        void SubmitFrame(DrawAssets& draw_assets) const override;

        void SubmitDebugInfos(const std::vector<UiDrawAsset>& debug_draw_assets) override;

        [[nodiscard]] uint32_t GetDrawCalls() const override;

    private:
        Environment::WindowContext m_window_context;
        std::unique_ptr<RenderFramework::IRenderer> m_renderer;
        std::unique_ptr<ShaderManagement::ShaderManager> m_shader_manager;
        std::vector<UiDrawAsset> m_debug_draw_assets;
    };
}
