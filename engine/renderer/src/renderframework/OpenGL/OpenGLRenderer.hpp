#pragma once
#include <GL/glew.h>
#include <glm/gtc/type_ptr.hpp>
#include <spdlog/spdlog.h>
#include <memory>
#include "Window.hpp"
#include "../Renderer.hpp"

#include "AssetHandler.hpp"
#include "OpenGlBinder.hpp"
#include "materials/OpenGlMaterialLibrary.hpp"
#include "Meshes/OpenGlMeshLibrary.hpp"
#include "Shaders/OpenGlShaderLibrary.hpp"
#include "Textures/OpenGLTextureLibrary.hpp"

namespace Engine::Renderer::RenderFramework::OpenGl
{
    class OpenGlRenderer final : public IRenderer
    {
    public:
        explicit OpenGlRenderer(const Environment::WindowContext& window_context,
                                AssetHandling::AssetHandler* asset_handler,
                                const std::shared_ptr<OpenGlMaterialLibrary>& material_library,
                                const std::shared_ptr<OpenGlShaderLibrary>& shader_library,
                                const std::shared_ptr<OpenGlMeshLibrary>& mesh_library,
                                const std::shared_ptr<OpenGLTextureLibrary>& texture_library);

        ~OpenGlRenderer() override;

        void Initialize() override;

        void PrepareFrame(const CameraAsset& camera_asset) override;

        void DrawFrame(std::vector<DrawAsset>& draw_assets) override;

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
        struct Context
        {
            AssetHandling::RenderState RenderPass;
            Assets::MaterialHandle Material;
            ShaderBindings ShaderFields;
            Assets::MeshHandle Mesh;
            GLsizei MeshIndicesCount;
            glm::mat4 ProjectionMatrix;
        };

        CameraAsset m_camera_asset{};

        GLuint m_camera_ubo;
        static constexpr GLuint camera_binding_point = 0;
        std::unique_ptr<OpenGlBinder> m_bind_cache;

        std::shared_ptr<OpenGlMaterialLibrary> m_material_library;
        std::shared_ptr<OpenGlShaderLibrary> m_shader_manager;
        std::shared_ptr<OpenGlMeshLibrary> m_mesh_manager;
        std::shared_ptr<OpenGLTextureLibrary> m_texture_manager;
        AssetHandling::AssetHandler* m_asset_handler;

        Context m_context{};

        glm::vec2 m_window_size{};
        uint32_t m_draw_calls = 0;

        static void SortDrawAssets(std::vector<DrawAsset>& mesh_draw_assets);

        void BindRenderPass(const AssetHandling::RenderState& render_state);
        void BindMaterial(const Assets::MaterialHandle& material);
        void BindMesh(const Assets::MeshHandle& mesh_handle);
        void BindShaders(const Assets::ShaderHandle& shader);
        void DrawElement(const glm::mat4& model_matrix);
    };
} // namespace
