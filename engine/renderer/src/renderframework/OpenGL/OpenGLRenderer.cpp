#include "OpenGLRenderer.hpp"

#include <ranges>
#include <vector>
#include <algorithm>

namespace Engine::Renderer::RenderFramework::OpenGl
{
    static auto MakeDrawAssetSortKey(const DrawAsset& mda)
    {
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
                                   std::unique_ptr<Materials::MaterialLibrary> material_library)
        :
        IRenderer(std::move(material_library))
    {
        glewExperimental = GL_TRUE;
        const GLenum rc = glewInit();
        if (rc != GLEW_OK)
        {
            throw std::runtime_error("Failed to initialize GLEW");
        }
        m_window_size = {window_context.width, window_context.height};
        glViewport(0, 0, window_context.drawableWidth, window_context.drawableHeight);
        m_bind_cache = std::make_unique<OpenGlBinder>();
        m_shader_manager = std::make_unique<OpenGlShaderManager>(asset_handler);
        m_mesh_manager = std::make_unique<OpenGlMeshManager>();
        m_texture_manager = std::make_unique<OpenGLTextureManager>();
        m_asset_handler = asset_handler;
        m_camera_asset = {};
        m_camera_ubo = 0;
    }

    OpenGlRenderer::~OpenGlRenderer() = default;

    void OpenGlRenderer::Initialize()
    {
        m_shader_manager->CompileShaders();
        glGenBuffers(1, &m_camera_ubo);
        glBindBuffer(GL_UNIFORM_BUFFER, m_camera_ubo);
        glBufferData(GL_UNIFORM_BUFFER, sizeof(CameraAsset), nullptr, GL_DYNAMIC_DRAW);
        glBindBuffer(GL_UNIFORM_BUFFER, 0);
        glBindBufferBase(GL_UNIFORM_BUFFER, camera_binding_point, m_camera_ubo);
        const auto shader_handle = m_asset_handler->GetHandleFromName<AssetHandling::ShaderAsset>("mesh_opaque");
        const auto shader_program = m_shader_manager->GetShaderProgram(shader_handle);
        if (!shader_program.has_value())
        {
            throw std::runtime_error("Shader program not found");
        }
        const GLuint cam_block_index = glGetUniformBlockIndex(shader_program.value(), "Camera");
        if (cam_block_index != GL_INVALID_INDEX)
        {
            glUniformBlockBinding(shader_program.value(), cam_block_index, camera_binding_point);
        }
        glEnable(GL_MULTISAMPLE);
        int samples = 0;
        glGetIntegerv(GL_SAMPLES, &samples);
        spdlog::info("MSAA samples: {}", samples);
    }

    void OpenGlRenderer::PrepareFrame(const CameraAsset& camera_asset)
    {
        glBindBuffer(GL_UNIFORM_BUFFER, m_camera_ubo);
        glBufferSubData(GL_UNIFORM_BUFFER, 0, static_cast<GLsizeiptr>(sizeof(CameraAsset)), &camera_asset);
        glBindBuffer(GL_UNIFORM_BUFFER, 0);
        glClearColor(0.1f, 0.1f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void OpenGlRenderer::DrawFrame(std::vector<DrawAsset>& draw_assets)
    {
        m_draw_calls = 0;
        SortDrawAssets(draw_assets);
        for (auto draw_asset : draw_assets)
        {
            BindRenderPass(draw_asset.RenderState);
            BindMaterial(draw_asset.Material);
            // m_bind_cache->BindShader();
            m_bind_cache->BindColor(m_context.ShaderFields, draw_asset.Color);
            BindMesh(draw_asset.Mesh);
            DrawElement(draw_asset.Model);
        }
        glBindVertexArray(0);
        glUseProgram(0);
    }

    void OpenGlRenderer::AddMesh(const AssetHandling::MeshAsset& mesh, const Assets::MeshHandle& handle)
    {
        return m_mesh_manager->AddMesh(mesh, handle);
    }

    void OpenGlRenderer::RemoveMesh(const Assets::MeshHandle& mesh_handle)
    {
        m_mesh_manager->RemoveMesh(mesh_handle);
    }

    void OpenGlRenderer::AddTexture(const AssetHandling::TextureAsset& texture, const Assets::TextureHandle& handle)
    {
        return m_texture_manager->AddTexture(texture, handle);
    }

    void OpenGlRenderer::RemoveTexture(const Assets::TextureHandle& texture_handle)
    {
        m_texture_manager->RemoveTexture(texture_handle);
    }

    void OpenGlRenderer::Shutdown()
    {
        m_shader_manager.reset();
        m_mesh_manager->Clear();
        m_texture_manager->Clear();
    }

    void OpenGlRenderer::SortDrawAssets(std::vector<DrawAsset>& mesh_draw_assets)
    {
        std::ranges::sort(mesh_draw_assets, [](const DrawAsset& a, const DrawAsset& b)
        {
            const auto key_a = MakeDrawAssetSortKey(a);
            const auto key_b = MakeDrawAssetSortKey(b);
            return key_a < key_b;
        });
    }

    void OpenGlRenderer::BindRenderPass(const AssetHandling::RenderState& render_state)
    {
        if (m_context.RenderPass != render_state)
        {
            glBindVertexArray(0);
            glUseProgram(0);
            m_context.Material = {};
            m_context.ShaderFields = {};
            m_context.Mesh = {};
            switch (render_state)
            {
                case AssetHandling::RenderState::Opaque: m_bind_cache->BindOpaquePassParameters();
                    m_context.ProjectionMatrix = glm::mat4(1.0);
                    break;
                case AssetHandling::RenderState::UI: m_bind_cache->BindUiPassParameters();
                    m_context.ProjectionMatrix = glm::ortho(0.f, m_window_size.x, m_window_size.y, 0.f, -1.0f, 1.0f);
                    break;
                default: throw std::runtime_error("Unknown render state");
            }
            m_context.RenderPass = render_state;
        }
    }

    void OpenGlRenderer::BindMaterial(const Assets::MaterialHandle& material_handle)
    {
        if (m_context.Material == material_handle || !material_handle)
        {
            return;
        }
        m_context.Material = material_handle;
        const auto& material = m_material_library->Get(material_handle);
        BindShaders(material.shader);
        GLuint texture = 0;
        if (material.albedo_texture.texture)
        {
            const auto& texture_ref = m_texture_manager->GetTexture(material.albedo_texture.texture);
            texture = texture_ref.texture_id;
        }
        m_bind_cache->BindAlbedoTexture(m_context.ShaderFields, texture);
    }

    void OpenGlRenderer::BindMesh(const Assets::MeshHandle& mesh_handle)
    {
        if (m_context.Mesh == mesh_handle)
        {
            return;
        }

        const auto& mesh = m_mesh_manager->GetMesh(mesh_handle);
        m_context.Mesh = mesh_handle;
        m_context.MeshIndicesCount = m_bind_cache->BindMesh(mesh);
    }

    void OpenGlRenderer::BindShaders(const Assets::ShaderHandle& shader)
    {
        const auto shader_program = m_shader_manager->GetShaderProgram(shader);
        if (!shader_program.has_value())
        {
            throw std::runtime_error("Shader program not found");
        }
        m_context.ShaderFields = m_bind_cache->BindShaderFields(shader_program.value());
        m_bind_cache->BindShader();
    }

    void OpenGlRenderer::DrawElement(const glm::mat4& model_matrix)
    {
        if (m_context.MeshIndicesCount == 0)
        {
            return;
        }
        const glm::mat4 matrix = m_context.ProjectionMatrix * model_matrix;
        m_bind_cache->BindMatrix(m_context.ShaderFields, matrix);
        glDrawElements(GL_TRIANGLES, m_context.MeshIndicesCount, GL_UNSIGNED_INT, nullptr);
        m_draw_calls++;
    }
}
