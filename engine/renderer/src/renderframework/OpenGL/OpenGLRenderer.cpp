#include "OpenGLRenderer.hpp"

#include <ranges>
#include <vector>
#include <algorithm>

namespace Engine::Renderer::RenderFramework::OpenGl {
    static auto MakeDrawAssetSortKey(const DrawAsset& mda) {
        return std::tuple{
            static_cast<uint8_t>(mda.RenderState),
            mda.RenderQueueIndex,
            mda.Material.value,
            mda.Mesh.value,
            mda.Entity,
        };
    }

    OpenGlRenderer::OpenGlRenderer(const Environment::WindowContext& window_context,
                                   AssetHandling::AssetHandler* asset_handler,
                                   const std::shared_ptr<OpenGlMaterialLibrary>& material_library,
                                   const std::shared_ptr<OpenGlShaderLibrary>& shader_library,
                                   const std::shared_ptr<OpenGlMeshLibrary>& mesh_library,
                                   const std::shared_ptr<OpenGLTextureLibrary>& texture_library) {
        glewExperimental = GL_TRUE;
        const GLenum rc = glewInit();
        if (rc != GLEW_OK) {
            throw std::runtime_error("Failed to initialize GLEW");
        }
        m_window_size = {window_context.width, window_context.height};
        glViewport(0, 0, window_context.drawableWidth, window_context.drawableHeight);
        m_bind_cache = std::make_unique<OpenGlBinder>();
        m_material_library = material_library;
        m_shader_manager = shader_library;
        m_mesh_manager = mesh_library;
        m_texture_manager = texture_library;
        m_asset_handler = asset_handler;
        m_camera_asset = {};
        m_camera_ubo = 0;
    }

    OpenGlRenderer::~OpenGlRenderer() = default;

    void OpenGlRenderer::Initialize() {
        const auto shader_handles = m_asset_handler->GetAllAssetHandlesOfType<AssetHandling::ShaderAsset>();
        std::vector<std::tuple<Assets::ShaderHandle, std::shared_ptr<const AssetHandling::ShaderAsset> > > shaders;
        shaders.reserve(shader_handles.size());
        for (int i = 0; i < shader_handles.size(); ++i) {
            const auto shader_handle = shader_handles[i];
            const auto asset = m_asset_handler->GetAsset<AssetHandling::ShaderAsset>(shader_handle);
            const auto tuple = std::make_tuple(shader_handle, asset);
            shaders.push_back(tuple);
        }

        m_shader_manager->CompileShaders(shaders);
        glGenBuffers(1, &m_camera_ubo);
        glBindBuffer(GL_UNIFORM_BUFFER, m_camera_ubo);
        glBufferData(GL_UNIFORM_BUFFER, sizeof(CameraAsset), nullptr, GL_DYNAMIC_DRAW);
        glBindBuffer(GL_UNIFORM_BUFFER, 0);
        glBindBufferBase(GL_UNIFORM_BUFFER, camera_binding_point, m_camera_ubo);
        const auto shader_handle = m_asset_handler->GetHandleFromName<AssetHandling::ShaderAsset>("mesh_opaque");
        const auto shader_program = m_shader_manager->GetShaderProgram(shader_handle);
        if (!shader_program.has_value()) {
            throw std::runtime_error("Shader program not found");
        }
        const GLuint cam_block_index = glGetUniformBlockIndex(shader_program.value(), "Camera");
        if (cam_block_index != GL_INVALID_INDEX) {
            glUniformBlockBinding(shader_program.value(), cam_block_index, camera_binding_point);
        }
        glEnable(GL_MULTISAMPLE);
        int samples = 0;
        glGetIntegerv(GL_SAMPLES, &samples);
        spdlog::info("MSAA samples: {}", samples);
    }

    void OpenGlRenderer::PrepareFrame(const CameraAsset& camera_asset) {
        glBindBuffer(GL_UNIFORM_BUFFER, m_camera_ubo);
        glBufferSubData(GL_UNIFORM_BUFFER, 0, static_cast<GLsizeiptr>(sizeof(CameraAsset)), &camera_asset);
        glBindBuffer(GL_UNIFORM_BUFFER, 0);
        glClearColor(0.1f, 0.1f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void OpenGlRenderer::DrawFrame(std::vector<DrawAsset>& draw_assets) {
        m_draw_calls = 0;
        LightAsset light_asset{
            .position = glm::vec3(2.0f, 1.0f, 0.0f),
            .color = glm::vec3(1.0f, 1.0f, 1.0f),
            .ambient_intensity = 0.1f,
        };
        SortDrawAssets(draw_assets);
        for (auto draw_asset: draw_assets) {
            BindRenderPass(draw_asset.RenderState);
            BindMaterial(draw_asset.Material);
            m_bind_cache->BindColor(m_context.ShaderFields, draw_asset.Color);
            BindMesh(draw_asset.Mesh);
            m_bind_cache->BindLight(m_context.ShaderFields,
                                    light_asset.position,
                                    light_asset.color,
                                    light_asset.ambient_intensity
                    );

            DrawElement(draw_asset.Model);
        }
        glBindVertexArray(0);
        glUseProgram(0);
    }

    void OpenGlRenderer::Shutdown() {
        m_mesh_manager->ClearMeshes();
        m_texture_manager->ClearTextures();
        m_shader_manager->ClearShaders();
        m_shader_manager.reset();
        m_material_library->ClearMaterials();
    }

    void OpenGlRenderer::SortDrawAssets(std::vector<DrawAsset>& mesh_draw_assets) {
        std::ranges::sort(mesh_draw_assets,
                          [](const DrawAsset& a, const DrawAsset& b) {
                              const auto key_a = MakeDrawAssetSortKey(a);
                              const auto key_b = MakeDrawAssetSortKey(b);
                              return key_a < key_b;
                          }
                );
    }

    void OpenGlRenderer::BindRenderPass(const AssetHandling::RenderState& render_state) {
        if (m_context.RenderPass != render_state) {
            glBindVertexArray(0);
            glUseProgram(0);
            m_context.Material = {};
            m_context.ShaderFields = {};
            m_context.Mesh = {};
            switch (render_state) {
                case AssetHandling::RenderState::Opaque:
                    m_bind_cache->BindOpaquePassParameters();
                    m_context.ProjectionMatrix = glm::mat4(1.0);
                    break;
                case AssetHandling::RenderState::UI:
                    m_bind_cache->BindUiPassParameters();
                    m_context.ProjectionMatrix = glm::ortho(0.f, m_window_size.x, m_window_size.y, 0.f, -1.0f, 1.0f);
                    break;
                default:
                    throw std::runtime_error("Unknown render state");
            }
            m_context.RenderPass = render_state;
        }
    }

    void OpenGlRenderer::BindMaterial(const Assets::MaterialHandle& material_handle) {
        if (m_context.Material == material_handle || !material_handle) {
            return;
        }
        m_context.Material = material_handle;
        const auto& material = m_material_library->Get(material_handle);
        BindShaders(material.shader);
        GLuint texture = 0;
        if (material.albedo_texture.texture) {
            const auto& texture_ref = m_texture_manager->GetTexture(material.albedo_texture.texture);
            texture = texture_ref.texture_id;
        }
        m_bind_cache->BindAlbedoTexture(m_context.ShaderFields, texture);
        m_bind_cache->BindSpecularStrength(m_context.ShaderFields, 0.5f);
        m_bind_cache->BindShininess(m_context.ShaderFields, 0.3f);
    }

    void OpenGlRenderer::BindMesh(const Assets::MeshHandle& mesh_handle) {
        if (m_context.Mesh == mesh_handle) {
            return;
        }

        const auto& mesh = m_mesh_manager->GetMesh(mesh_handle);
        m_context.Mesh = mesh_handle;
        m_context.MeshIndicesCount = m_bind_cache->BindMesh(mesh);
    }

    void OpenGlRenderer::BindShaders(const Assets::ShaderHandle& shader) {
        const auto shader_program = m_shader_manager->GetShaderProgram(shader);
        if (!shader_program.has_value()) {
            throw std::runtime_error("Shader program not found");
        }
        m_context.ShaderFields = m_bind_cache->BindShaderFields(shader_program.value());
        m_bind_cache->BindShader();
    }

    void OpenGlRenderer::DrawElement(const glm::mat4& model_matrix) {
        if (m_context.MeshIndicesCount == 0) {
            return;
        }
        const glm::mat4 model_projection_matrix = m_context.ProjectionMatrix * model_matrix;
        const glm::mat3 normal_matrix = glm::transpose(glm::inverse(glm::mat3(model_matrix)));
        m_bind_cache->BindModelMatrix(m_context.ShaderFields, model_projection_matrix);
        m_bind_cache->BindNormalMatrix(m_context.ShaderFields, normal_matrix);
        glDrawElements(GL_TRIANGLES, m_context.MeshIndicesCount, GL_UNSIGNED_INT, nullptr);
        m_draw_calls++;

    }
}
