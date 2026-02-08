#pragma once
#include <string_view>
#include <GL/glew.h>
#include <glm/gtc/type_ptr.hpp>
#include <spdlog/spdlog.h>
#include "Window.hpp"
#include "../Renderer.hpp"

#include "AssetHandler.hpp"
#include "OpenGLMeshManager.hpp"
#include "OpenGLShaderManager.hpp"
#include "OpenGLTextureManager.hpp"

namespace Engine::Renderer::RenderFramework::OpenGl {
    class OpenGlRenderer final : public IRenderer {
    public:
        explicit OpenGlRenderer(const Environment::WindowContext& window_context,
                                AssetHandling::AssetHandler* asset_handler);

        ~OpenGlRenderer() override;

        void Initialize() override;

        void PrepareFrame(const CameraAsset& camera_asset) override;

        void DrawFrame(DrawAssets& draw_assets) override;

        MeshHandle AddMesh(const MeshAsset& mesh) override;

        void RemoveMesh(const MeshHandle& mesh_handle) override;

        TextureHandle AddTexture(const TextureAsset& texture) override;

        void RemoveTexture(const TextureHandle& texture_handle) override;

        uint32_t GetDrawCalls() override {
            return m_draw_calls;
        }

        void Shutdown() override;

    private:
        CameraAsset m_camera_asset{};

        GLuint m_camera_ubo;
        static constexpr GLuint camera_binding_point = 0;

        std::unique_ptr<OpenGlShaderManager> m_shader_manager;
        std::unique_ptr<OpenGlMeshManager> m_mesh_manager;
        std::unique_ptr<OpenGLTextureManager> m_texture_manager;

        glm::vec2 m_window_size{};
        uint32_t m_draw_calls = 0;
    };
} // namespace
