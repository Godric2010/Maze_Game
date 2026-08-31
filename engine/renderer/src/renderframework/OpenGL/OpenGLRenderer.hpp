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

namespace yarep::Renderer::RenderFramework::OpenGl {
    class OpenGlRenderer final : public IRenderer {
    public:
        explicit OpenGlRenderer(const Environment::WindowContext& window_context,
                                AssetHandling::AssetHandler* asset_handler,
                                const std::shared_ptr<OpenGlMaterialLibrary>& material_library,
                                const std::shared_ptr<OpenGlShaderLibrary>& shader_library,
                                const std::shared_ptr<OpenGlMeshLibrary>& mesh_library,
                                const std::shared_ptr<OpenGLTextureLibrary>& texture_library);

        ~OpenGlRenderer() override;

        void Initialize() override;

        void PrepareFrame(const FrameData& frame_data) override;

        void DrawFrame(std::vector<DrawAsset>& draw_assets) override;

        uint32_t GetDrawCalls() override {
            return m_draw_calls;
        }

        void Shutdown() override;

    private:
        struct Context {
            AssetHandling::RenderState RenderPass;
            assets::MaterialHandle Material;
            ShaderBindings ShaderFields;
            assets::MeshHandle Mesh;
            GLsizei MeshIndicesCount;
            glm::mat4 ProjectionMatrix;
        };

        GLuint m_camera_ubo;
        static constexpr GLuint camera_binding_point = 0;

        GLuint m_lighting_ubo;
        static constexpr GLuint light_binding_point = 1;
        std::unique_ptr<OpenGlBinder> m_bind_cache;

        std::shared_ptr<OpenGlMaterialLibrary> m_material_library;
        std::shared_ptr<OpenGlShaderLibrary> m_shader_manager;
        std::shared_ptr<OpenGlMeshLibrary> m_mesh_manager;
        std::shared_ptr<OpenGLTextureLibrary> m_texture_manager;
        AssetHandling::AssetHandler* m_asset_handler;

        Context m_context{};

        glm::vec2 m_window_size{};
        uint32_t m_draw_calls = 0;

        void BindCamera(const CameraAsset& camera) const;

        void BindLights(const std::vector<LightAsset>& lights, const AmbientLightAsset& ambient) const;

        static void SortDrawAssets(std::vector<DrawAsset>& mesh_draw_assets);

        void BindRenderPass(const AssetHandling::RenderState& render_state);

        void BindMaterial(const assets::MaterialHandle& material);

        void BindMesh(const assets::MeshHandle& mesh_handle);

        void BindShaders(const assets::ShaderHandle& shader);

        void DrawElement(const glm::mat4& model_matrix);
    };
} // namespace
