#include "OpenGLRenderer.hpp"

#include <ostream>

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
    }

    void OpenGLRenderer::Render() {
        glClearColor(0.1f, 0.1f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(m_shaderProgram);
        for (const auto & mesh : m_meshes) {
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
        glBufferData(GL_ARRAY_BUFFER, size , mesh.vertices.data(), GL_STATIC_DRAW);

        // Bind index buffer
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m.EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<GLsizeiptr>(sizeof(uint32_t)* m.numIndices),mesh.indices.data(),GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3),static_cast<void *>(0));

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
        m_meshes.emplace_back(m);
    }

    void OpenGLRenderer::RemoveMesh() {
    }

    void OpenGLRenderer::Shutdown() {
        glDeleteProgram(m_shaderProgram);
        for (auto & meshes : m_meshes) {
            glDeleteBuffers(1, &meshes.VBO);
            glDeleteVertexArrays(1, &meshes.VAO);
        }
    }

    unsigned int OpenGLRenderer::LoadShaders() const {
        auto shader = m_shaderManager->GetShader("test");
        if (!shader.has_value()) {
            throw std::runtime_error("Failed to load shader");
        }

        const char *vSrc = shader.value().vertexShader.c_str();
        const char *fSrc = shader.value().fragmentShader.c_str();

        const GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertexShader, 1, &vSrc, nullptr);
        glCompileShader(vertexShader);

        const GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentShader, 1, &fSrc, nullptr);
        glCompileShader(fragmentShader);

        const GLuint shaderProgram = glCreateProgram();
        glAttachShader(shaderProgram, vertexShader);
        glAttachShader(shaderProgram, fragmentShader);
        glLinkProgram(shaderProgram);

        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);

        return shaderProgram;
    }
} // namespace
