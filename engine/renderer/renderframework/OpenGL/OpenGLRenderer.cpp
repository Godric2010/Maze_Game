#include "OpenGLRenderer.hpp"

#include <vector>

#include "../../../core/components/Mesh.hpp"

namespace Engine::Renderer::RenderFramework::OpenGl {
    OpenGlRenderer::OpenGlRenderer(const Environment::WindowContext &window_context,
                                   ShaderManagement::ShaderManager *shader_manager) {
        if (!gladLoadGLLoader(SDL_GL_GetProcAddress)) {
            throw std::runtime_error("Failed to initialize OpenGL context");
        }

        m_window_size = {window_context.width, window_context.height};
        glViewport(0, 0, window_context.drawableWidth, window_context.drawableHeight);
        m_shader_manager = std::make_unique<OpenGlShaderManager>(shader_manager);
        m_mesh_manager = std::make_unique<OpenGlMeshManager>();

        m_camera_asset = {};
        m_camera_ubo = 0;
    }

    OpenGlRenderer::~OpenGlRenderer() = default;

    void OpenGlRenderer::Initialize() {
        m_shader_manager->CompileShaders();

        glGenBuffers(1, &m_camera_ubo);
        glBindBuffer(GL_UNIFORM_BUFFER, m_camera_ubo);
        glBufferData(GL_UNIFORM_BUFFER, sizeof(CameraAsset), nullptr, GL_DYNAMIC_DRAW);
        glBindBuffer(GL_UNIFORM_BUFFER, 0);

        glBindBufferBase(GL_UNIFORM_BUFFER, camera_binding_point, m_camera_ubo);

        const auto shader_program = m_shader_manager->GetShaderProgram("mesh_opaque");
        if (!shader_program.has_value()) {
            throw std::runtime_error("Shader program not found");
        }

        const GLuint cam_block_index = glGetUniformBlockIndex(shader_program.value(), "Camera");
        if (cam_block_index != GL_INVALID_INDEX) {
            glUniformBlockBinding(shader_program.value(), cam_block_index, camera_binding_point);
        }
    }

    void OpenGlRenderer::PrepareFrame(const CameraAsset &camera_asset) {
        glBindBuffer(GL_UNIFORM_BUFFER, m_camera_ubo);
        glBufferSubData(GL_UNIFORM_BUFFER, 0, static_cast<GLsizeiptr>(sizeof(CameraAsset)), &camera_asset);
        glBindBuffer(GL_UNIFORM_BUFFER, 0);

        glClearColor(0.1f, 0.1f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }


    void OpenGlRenderer::DrawFrame(DrawAssets &draw_assets) {
        // Draw meshes in first pass
        std::vector<std::vector<const MeshDrawAsset *> > buckets(m_mesh_manager->Size());
        for (const auto &draw_asset: draw_assets.mesh_draw_assets) {
            buckets[draw_asset.mesh].push_back(&draw_asset);
        }
        glDisable(GL_BLEND);
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);
        const auto shader_program = m_shader_manager->GetShaderProgram("mesh_opaque");
        if (!shader_program.has_value()) {
            throw std::runtime_error("Shader program not found");
        }
        const GLint u_model = glGetUniformLocation(shader_program.value(), "u_Model");
        const GLint u_color = glGetUniformLocation(shader_program.value(), "u_Color");
        glUseProgram(shader_program.value());

        for (MeshHandle h = 0; h < buckets.size(); ++h) {
            const auto &list = buckets[h];
            if (list.empty()) {
                continue;
            }

            const auto &mesh = m_mesh_manager->GetMesh(h);
            glBindVertexArray(mesh.VAO);

            for (const MeshDrawAsset *draw_asset: list) {
                glUniformMatrix4fv(u_model, 1, GL_FALSE, glm::value_ptr(draw_asset->model));
                glUniform4fv(u_color, 1, glm::value_ptr(draw_asset->color));

                glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(mesh.numIndices), GL_UNSIGNED_INT, nullptr);
            }
        }
        glBindVertexArray(0);
        glUseProgram(0);

        // Render UI Elements
        glDisable(GL_DEPTH_TEST);
        glDisable(GL_CULL_FACE);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        const auto ui_shader_program = m_shader_manager->GetShaderProgram("ui");
        if (!shader_program.has_value()) {
            throw std::runtime_error("Shader program not found");
        }
        const GLint u_proj = glGetUniformLocation(ui_shader_program.value(), "u_Proj");
        const GLint u_ui_color = glGetUniformLocation(shader_program.value(), "u_Color");
        glUseProgram(ui_shader_program.value());

        const auto &mesh = m_mesh_manager->GetMesh(0);
        glBindVertexArray(mesh.VAO);
        const glm::mat4 ortho = glm::ortho(0.f, m_window_size.x, m_window_size.y, 0.f, -1.f, 1.f);
        for (const auto &ui_asset: draw_assets.ui_draw_assets) {
            glm::mat4 model = ui_asset.model;
            glm::mat4 proj = ortho * model;

            glUniformMatrix4fv(u_proj, 1, GL_FALSE, glm::value_ptr(proj));
            glUniform4fv(u_ui_color, 1, glm::value_ptr(ui_asset.color));

            glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(mesh.numIndices), GL_UNSIGNED_INT, nullptr);
        }
        glBindVertexArray(0);
        glUseProgram(0);
    }

    MeshHandle OpenGlRenderer::AddMesh(const MeshAsset &mesh) {
        return m_mesh_manager->AddMesh(mesh);
    }

    void OpenGlRenderer::RemoveMesh(const MeshHandle &mesh_handle) {
        m_mesh_manager->RemoveMesh(mesh_handle);
    }

    void OpenGlRenderer::Shutdown() {
        m_shader_manager.reset();
        m_mesh_manager->Clear();
    }
} // namespace
