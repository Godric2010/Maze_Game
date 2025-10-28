#include "OpenGLRenderer.hpp"

#include <vector>

#include "../../../core/components/Mesh.hpp"

namespace Engine::Renderer::RenderFramework::OpenGl {
    OpenGlRenderer::OpenGlRenderer(const Environment::WindowContext &window_context,
                                   ShaderManagement::ShaderManager *shader_manager) {
        if (!gladLoadGLLoader(SDL_GL_GetProcAddress)) {
            throw std::runtime_error("Failed to initialize OpenGL context");
        }

        glViewport(0, 0, window_context.drawableWidth, window_context.drawableHeight);
        m_shader_manager = shader_manager;
        m_mesh_manager = std::make_unique<OpenGLMeshManager>();

        m_camera_asset = {};
        m_camera_ubo = 0;
        m_shader_program = 0;
    }

    OpenGlRenderer::~OpenGlRenderer() = default;

    void OpenGlRenderer::Initialize() {
        m_shader_program = LoadShaders();

        glGenBuffers(1, &m_camera_ubo);
        glBindBuffer(GL_UNIFORM_BUFFER, m_camera_ubo);
        glBufferData(GL_UNIFORM_BUFFER, sizeof(CameraAsset), nullptr, GL_DYNAMIC_DRAW);
        glBindBuffer(GL_UNIFORM_BUFFER, 0);

        glBindBufferBase(GL_UNIFORM_BUFFER, camera_binding_point, m_camera_ubo);
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);

        const GLuint camBlockIndex = glGetUniformBlockIndex(m_shader_program, "Camera");
        if (camBlockIndex != GL_INVALID_INDEX) {
            glUniformBlockBinding(m_shader_program, camBlockIndex, camera_binding_point);
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
        std::vector<std::vector<const MeshDrawAsset *> > buckets(m_mesh_manager->Size());
        for (const auto &draw_asset: draw_assets.mesh_draw_assets) {
            buckets[draw_asset.mesh].push_back(&draw_asset);
        }

        const GLint u_model = glGetUniformLocation(m_shader_program, "u_Model");
        const GLint u_color = glGetUniformLocation(m_shader_program, "u_Color");
        glUseProgram(m_shader_program);

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
    }

    MeshHandle OpenGlRenderer::AddMesh(const MeshAsset &mesh) {
        return m_mesh_manager->AddMesh(mesh);
    }

    void OpenGlRenderer::RemoveMesh(const MeshHandle &mesh_handle) {
        m_mesh_manager->RemoveMesh(mesh_handle);
    }

    void OpenGlRenderer::Shutdown() {
        glDeleteProgram(m_shader_program);
        m_mesh_manager->Clear();
    }

    void OpenGlRenderer::LogSourceWithLineNumbers(const std::string_view source) {
        std::istringstream iss{std::string(source)};
        std::string line;
        int line_number = 1;
        while (std::getline(iss, line)) {
            spdlog::info("{:4d}: {}", line_number++, line);
        }
    }

    GLuint OpenGlRenderer::LoadShaders() const {
        constexpr std::string shader_name = "test";
        const auto shader = m_shader_manager->GetShader(shader_name);
        if (!shader.has_value()) {
            throw std::runtime_error("Failed to load shader");
        }

        const char *v_src = shader.value().vertexShader.c_str();
        const char *f_src = shader.value().fragmentShader.c_str();

        const auto vertex_shader = CompileShader(GL_VERTEX_SHADER, v_src, shader_name + ".vertexShader");
        const auto fragment_shader = CompileShader(GL_FRAGMENT_SHADER, f_src, shader_name + ".fragmentShader");

        const auto shader_program = LinkShaderProgram(vertex_shader, fragment_shader, shader_name);
        return shader_program;
    }

    GLuint OpenGlRenderer::CompileShader(const GLenum type, const std::string_view source,
                                         const std::string_view debug_name) {
        const GLuint shader = glCreateShader(type);
        const char *ptr = source.data();
        const auto length = static_cast<GLint>(source.size());
        glShaderSource(shader, 1, &ptr, &length);
        glCompileShader(shader);

        GLint status = GL_FALSE;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &status);

        if (status != GL_TRUE) {
            GLint log_length = 0;
            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &log_length);
            const std::string log(log_length > 1 ? log_length - 1 : 0, '\0');
            spdlog::error("[Shader Compile] {} failed: \n{}", std::string(debug_name).c_str(), log.c_str());
            LogSourceWithLineNumbers(source);

            glDeleteShader(shader);
            throw std::runtime_error("Failed to compile shader " + std::string(debug_name));
        }
        return shader;
    }


    GLuint OpenGlRenderer::LinkShaderProgram(const GLuint vertex_shader, const GLuint fragment_shader,
                                             const std::string_view debug_name) {
        const GLuint program = glCreateProgram();
        glAttachShader(program, vertex_shader);
        glAttachShader(program, fragment_shader);
        glLinkProgram(program);

        GLint status = GL_FALSE;
        glGetProgramiv(program, GL_LINK_STATUS, &status);

        if (status != GL_TRUE) {
            GLint log_length = 0;
            glGetProgramiv(program, GL_INFO_LOG_LENGTH, &log_length);
            std::string log(log_length > 1 ? log_length - 1 : 0, '\0');
            if (log_length > 1) {
                glGetProgramInfoLog(program, log_length, nullptr, reinterpret_cast<GLchar *>(log.data()));
            }
            spdlog::error("[Program Link] {} failed: \n{}", std::string(debug_name).c_str(), log.c_str());

            glDeleteProgram(program);
            throw std::runtime_error("Failed to link shader program " + std::string(debug_name));
        }

        glDetachShader(program, vertex_shader);
        glDetachShader(program, fragment_shader);
        glDeleteShader(vertex_shader);
        glDeleteShader(fragment_shader);

        return program;
    }
} // namespace
