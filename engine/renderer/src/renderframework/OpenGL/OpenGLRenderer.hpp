#pragma once
#include <GL/glew.h>
#include <glm/gtc/type_ptr.hpp>
#include <spdlog/spdlog.h>
#include <memory>
#include "Window.hpp"
#include "../Renderer.hpp"

#include "AssetHandler.hpp"
#include "OpenGlBinder.hpp"
#include "OpenGLMeshManager.hpp"
#include "OpenGLShaderManager.hpp"
#include "OpenGLTextureManager.hpp"

namespace Engine::Renderer::RenderFramework::OpenGl
{
    class OpenGlRenderer final : public IRenderer
    {
    public:
        explicit OpenGlRenderer(const Environment::WindowContext& window_context,
                                AssetHandling::AssetHandler* asset_handler,
                                std::unique_ptr<Materials::MaterialLibrary> material_library);

        ~OpenGlRenderer() override;

        void Initialize() override;

        void PrepareFrame(const CameraAsset& camera_asset) override;
        void RenderUIPass(const std::vector<UiDrawAsset>& draw_assets);

        void DrawFrame(DrawAssets& draw_assets) override;

        void AddMesh(const AssetHandling::MeshAsset& mesh, const Assets::MeshHandle& handle) override;

        void RemoveMesh(const Assets::MeshHandle& mesh_handle) override;

        void AddTexture(const AssetHandling::TextureAsset& texture, const Assets::TextureHandle& handle) override;

        void RemoveTexture(const Assets::TextureHandle& texture_handle) override;

        uint32_t GetDrawCalls() override
        {
            return m_draw_calls;
        }

        void Shutdown() override;

    private:
        CameraAsset m_camera_asset{};

        GLuint m_camera_ubo;
        static constexpr GLuint camera_binding_point = 0;

        std::unique_ptr<OpenGlBinder> m_bind_cache;
        std::unique_ptr<OpenGlShaderManager> m_shader_manager;
        std::unique_ptr<OpenGlMeshManager> m_mesh_manager;
        std::unique_ptr<OpenGLTextureManager> m_texture_manager;
        AssetHandling::AssetHandler* m_asset_handler;
        ShaderBindings m_current_shader_bindings{};

        glm::vec2 m_window_size{};
        uint32_t m_draw_calls = 0;

        static void SortDrawAssets(std::vector<MeshDrawAsset>& mesh_draw_assets);

        void RenderOpaquePass(const std::vector<MeshDrawAsset>& mesh_draw_assets);
        void BindMaterial(const Materials::Material& material) ;
        static void BindMesh(const OpenGLMesh& mesh, GLsizei& mesh_indices_count);
        void BindShaders(const Assets::ShaderHandle& shader);
    };
} // namespace
