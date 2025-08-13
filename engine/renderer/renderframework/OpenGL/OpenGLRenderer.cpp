#include "OpenGLRenderer.hpp"

#include "../../meshmanagement/Mesh.hpp"

namespace Engine::Renderer::RenderFramework::OpenGL {
    OpenGLRenderer::OpenGLRenderer(Window::WindowContext windowContext,
                                   ShaderManagement::ShaderManager *shaderManager) {
        if (!gladLoadGLLoader((GLADloadproc) SDL_GL_GetProcAddress)) {
            throw std::runtime_error("Failed to initialize OpenGL context");
        }

        m_shaderManager = shaderManager;

        m_meshes = std::vector<OpenGLMesh>();
        m_shaderProgram = 0;
    }

    OpenGLRenderer::~OpenGLRenderer() = default;

    void OpenGLRenderer::Initialize() {
        m_shaderProgram = LoadShaders();

        glGenBuffers(1, &m_cameraUBO);
        glBindBuffer(GL_UNIFORM_BUFFER, m_cameraUBO);
        glBufferData(GL_UNIFORM_BUFFER, sizeof(CameraGPUData), nullptr, GL_DYNAMIC_DRAW);
        glBindBuffer(GL_UNIFORM_BUFFER, 0);

        glBindBufferBase(GL_UNIFORM_BUFFER, CAMERA_BINDING_POINT, m_cameraUBO);
        glEnable(GL_DEPTH_TEST);

        const GLuint camBlockIndex = glGetUniformBlockIndex(m_shaderProgram, "Camera");
        if (camBlockIndex == GL_INVALID_INDEX) {
            glUniformBlockBinding(m_shaderProgram, camBlockIndex, CAMERA_BINDING_POINT);
        }
    }

    void OpenGLRenderer::PrepareFrame(const glm::mat4 camView, const glm::mat4 camProj, const glm::vec3 camPos) {
        CameraGPUData camData{};
        camData.camView = camView;
        camData.camProj = camProj;
        camData.camPos = glm::vec4(camPos, 1.0f);

        glBindBuffer(GL_UNIFORM_BUFFER, m_cameraUBO);
        glBufferSubData(GL_UNIFORM_BUFFER, 0, static_cast<GLsizeiptr>(sizeof(CameraGPUData)), &camData);
        glBindBuffer(GL_UNIFORM_BUFFER, 0);

        glClearColor(0.1f, 0.1f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }


    void OpenGLRenderer::DrawFrame() {
        glUseProgram(m_shaderProgram);

        for (const auto &mesh: m_meshes) {
            glBindVertexArray(mesh.VAO);
            glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(mesh.numIndices), GL_UNSIGNED_INT, nullptr);
        }

        glBindVertexArray(0);
        glUseProgram(0);
    }

    void OpenGLRenderer::AddMesh(const Meshmanagement::Mesh &mesh) {
        OpenGLMesh m = {};
        m.numVertices = mesh.vertices.size();
        m.numIndices = mesh.indices.size();

        glGenVertexArrays(1, &m.VAO);
        glGenBuffers(1, &m.VBO);
        glGenBuffers(1, &m.EBO);

        glBindVertexArray(m.VAO);

        // Bind vertex buffer
        glBindBuffer(GL_ARRAY_BUFFER, m.VBO);
        const auto size = static_cast<GLsizeiptr>(m.numVertices * sizeof(glm::vec3));
        glBufferData(GL_ARRAY_BUFFER, size, mesh.vertices.data(), GL_STATIC_DRAW);

        // Bind index buffer
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m.EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<GLsizeiptr>(sizeof(uint32_t) * m.numIndices),
                     mesh.indices.data(),GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), static_cast<void *>(0));

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
        m_meshes.emplace_back(m);
    }

    void OpenGLRenderer::RemoveMesh() {
    }

    void OpenGLRenderer::Shutdown() {
        glDeleteProgram(m_shaderProgram);
        for (auto &meshes: m_meshes) {
            glDeleteBuffers(1, &meshes.VBO);
            glDeleteVertexArrays(1, &meshes.VAO);
        }
    }

    void OpenGLRenderer::logSourceWithLineNumbers(std::string_view source) {
        std::istringstream iss{std::string(source)};
        std::string line;
        int lineNumber = 1;
        while (std::getline(iss, line)) {
            spdlog::info("{:4d}: {}", lineNumber++, line);
        }
    }

    GLuint OpenGLRenderer::LoadShaders() const {
        const std::string shaderName = "test";
        const auto shader = m_shaderManager->GetShader(shaderName);
        if (!shader.has_value()) {
            throw std::runtime_error("Failed to load shader");
        }

        const char *vSrc = shader.value().vertexShader.c_str();
        const char *fSrc = shader.value().fragmentShader.c_str();

        const auto vertexShader = CompileShader(GL_VERTEX_SHADER, vSrc, shaderName + ".vertexShader");
        const auto fragmentShader = CompileShader(GL_FRAGMENT_SHADER, fSrc, shaderName + ".fragmentShader");

        const auto shaderProgram = LinkShaderProgram(vertexShader, fragmentShader, shaderName);
        return shaderProgram;
    }

    GLuint OpenGLRenderer::CompileShader(const GLenum type, const std::string_view source,
                                         const std::string_view debugName) {
        const GLuint shader = glCreateShader(type);
        const char *ptr = source.data();
        const auto length = static_cast<GLint>(source.size());
        glShaderSource(shader, 1, &ptr, &length);
        glCompileShader(shader);

        GLint status = GL_FALSE;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &status);

        if (status != GL_TRUE) {
            GLint logLength = 0;
            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);
            const std::string log(logLength > 1 ? logLength - 1 : 0, '\0');
            spdlog::error("[Shader Compile] {} failed: \n{}", std::string(debugName).c_str(), log.c_str());
            logSourceWithLineNumbers(source);

            glDeleteShader(shader);
            throw std::runtime_error("Failed to compile shader " + std::string(debugName));
        }
        return shader;
    }


    GLuint OpenGLRenderer::LinkShaderProgram(const GLuint vertexShader, const GLuint fragmentShader,
                                             std::string_view debugName) {
        const GLuint program = glCreateProgram();
        glAttachShader(program, vertexShader);
        glAttachShader(program, fragmentShader);
        glLinkProgram(program);

        GLint status = GL_FALSE;
        glGetProgramiv(program, GL_LINK_STATUS, &status);

        if (status != GL_TRUE) {
            GLint logLength = 0;
            glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength);
            std::string log(logLength > 1 ? logLength - 1 : 0, '\0');
            if (logLength > 1) {
                glGetProgramInfoLog(program, logLength, nullptr, reinterpret_cast<GLchar *>(log.data()));
            }
            spdlog::error("[Program Link] {} failed: \n{}", std::string(debugName).c_str(), log.c_str());

            glDeleteProgram(program);
            throw std::runtime_error("Failed to link shader program " + std::string(debugName));
        }

        glDetachShader(program, vertexShader);
        glDetachShader(program, fragmentShader);
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);

        return program;
    }
} // namespace
