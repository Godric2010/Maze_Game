#include "OpenGLRenderer.hpp"

#include <ranges>
#include <vector>
#include <algorithm>

namespace Engine::Renderer::RenderFramework::OpenGl
{
    static auto MakeDrawAssetSortKey(const MeshDrawAsset& mda)
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
                                   std::unique_ptr<Materials::MaterialLibrary> material_library) : IRenderer(
        std::move(material_library))
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


    void OpenGlRenderer::DrawFrame(DrawAssets& draw_assets)
    {
        m_draw_calls = 0;

        SortDrawAssets(draw_assets.mesh_draw_assets);

        // Draw meshes in first pass
        RenderOpaquePass(draw_assets.mesh_draw_assets);

        // Render UI Elements
        RenderUIPass(draw_assets.ui_draw_assets);
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

    void OpenGlRenderer::SortDrawAssets(std::vector<MeshDrawAsset>& mesh_draw_assets)
    {
        std::sort(mesh_draw_assets.begin(), mesh_draw_assets.end(),
                  [](const MeshDrawAsset& a, const MeshDrawAsset& b)
                  {
                      const auto key_a = MakeDrawAssetSortKey(a);
                      const auto key_b = MakeDrawAssetSortKey(b);
                      return key_a < key_b;
                  }
        );
    }

    void OpenGlRenderer::RenderOpaquePass(const std::vector<MeshDrawAsset>& mesh_draw_assets)
    {
        if (mesh_draw_assets.empty())
        {
            return;
        }

        OpenGlBinder::BindOpaquePassParameters();

        auto last_material = Assets::MaterialHandle{0};
        auto last_mesh = Assets::MeshHandle{0};

        GLsizei mesh_index_count = 0;
        for (const auto& mesh_draw_asset : mesh_draw_assets)
        {
            if (mesh_draw_asset.Material != last_material)
            {
                const auto& material = m_material_library->Get(mesh_draw_asset.Material);
                BindMaterial(material);
                m_bind_cache->BindColor(m_current_shader_bindings, material.base_color);
                last_material = mesh_draw_asset.Material;
            }


            const auto& mesh = m_mesh_manager->GetMesh(mesh_draw_asset.Mesh);
            mesh_index_count = m_bind_cache->BindMesh(mesh);

            m_bind_cache->BindMatrix(m_current_shader_bindings, mesh_draw_asset.Model);
            glDrawElements(GL_TRIANGLES, mesh_index_count, GL_UNSIGNED_INT, nullptr);
            m_draw_calls++;
        }

        glBindVertexArray(0);
        glUseProgram(0);
    }

    void OpenGlRenderer::RenderUIPass(const std::vector<UiDrawAsset>& draw_assets)
    {
        OpenGlBinder::BindUiPassParameters();

        auto last_material = Assets::MaterialHandle{0};
        auto last_mesh = Assets::MeshHandle{0};


        GLsizei mesh_index_count = 0;
        const glm::mat4 ortho = glm::ortho(0.f, m_window_size.x, m_window_size.y, 0.f, -1.0f, 0.0f);
        for (const auto& draw_asset : draw_assets)
        {
            if (draw_asset.Material != last_material)
            {
                const auto& material = m_material_library->Get(draw_asset.Material);
                BindMaterial(material);

                last_material = draw_asset.Material;
            }
            m_bind_cache->BindColor(m_current_shader_bindings, draw_asset.color);

            const auto& mesh = m_mesh_manager->GetMesh(draw_asset.Mesh);
            mesh_index_count = m_bind_cache->BindMesh(mesh);
            glm::mat4 proj = ortho * draw_asset.Model;
            m_bind_cache->BindMatrix(m_current_shader_bindings, proj);
            glDrawElements(GL_TRIANGLES, mesh_index_count, GL_UNSIGNED_INT, nullptr);
            m_draw_calls++;
        }
        glBindVertexArray(0);
        glUseProgram(0);
    }

    void OpenGlRenderer::BindMaterial(const Materials::Material& material)
    {
        BindShaders(material.shader);

        GLuint texture = 0;
        if (material.albedo_texture.texture)
        {
            const auto& texture_ref = m_texture_manager->GetTexture(material.albedo_texture.texture);
            texture = texture_ref.texture_id;
        }
        m_bind_cache->BindAlbedoTexture(m_current_shader_bindings, texture);
    }

    void OpenGlRenderer::BindMesh(const OpenGLMesh& mesh, GLsizei& mesh_indices_count)
    {
        glBindVertexArray(mesh.VAO);
        mesh_indices_count = static_cast<GLsizei>(mesh.numIndices);
    }

    void OpenGlRenderer::BindShaders(const Assets::ShaderHandle& shader)
    {
        const auto shader_program = m_shader_manager->GetShaderProgram(shader);
        if (!shader_program.has_value())
        {
            throw std::runtime_error("Shader program not found");
        }
        m_current_shader_bindings = m_bind_cache->BindShader(shader_program.value());
    }
}
