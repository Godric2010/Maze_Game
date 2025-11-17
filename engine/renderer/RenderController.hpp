#pragma once
#include <memory>

#include "Window.hpp"
#include "renderframework/Renderer.hpp"
#include "shadermanagement/ShaderManager.hpp"
#include "Datatypes.hpp"
#include "PrimitiveMeshes.hpp"

namespace Engine::Renderer {
    class RenderController {
    public:
        explicit RenderController(const Environment::WindowContext &window_context);

        ~RenderController();

        [[nodiscard]] MeshHandle RegisterMesh(const MeshAsset &mesh) const;

        void UnregisterMesh(const MeshHandle &handle) const;

        [[nodiscard]] TextureHandle RegisterTexture(const TextureAsset &texture) const;

        void UnregisterTexture(const TextureHandle &handle) const;

        void BeginFrame(const CameraAsset &camera_asset) const;

        void SubmitFrame(DrawAssets &draw_assets) const;

    private:
        Environment::WindowContext m_window_context;
        std::unique_ptr<RenderFramework::IRenderer> m_renderer;
        std::unique_ptr<ShaderManagement::ShaderManager> m_shader_manager;
    };
}
