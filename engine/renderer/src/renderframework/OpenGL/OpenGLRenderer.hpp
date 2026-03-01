#pragma once
#include <GL/glew.h>
#include <glm/gtc/type_ptr.hpp>
#include <spdlog/spdlog.h>
#include <memory>
#include "Window.hpp"
#include "../Renderer.hpp"

#include "AssetHandler.hpp"
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

        std::unique_ptr<OpenGlShaderManager> m_shader_manager;
        std::unique_ptr<OpenGlMeshManager> m_mesh_manager;
        std::unique_ptr<OpenGLTextureManager> m_texture_manager;
        AssetHandling::AssetHandler* m_asset_handler;

        glm::vec2 m_window_size{};
        uint32_t m_draw_calls = 0;

        void RenderOpaquePass(const std::vector<MeshDrawAsset>& mesh_draw_assets);
        void BindMaterial(const Materials::Material& material, GLint& model_bind) const;
        void BindMeshes(const std::vector<const MeshDrawAsset*>& meshes, const GLint& model_bind);
    };
} // namespace
