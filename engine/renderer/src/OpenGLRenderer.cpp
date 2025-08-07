#include "OpenGLRenderer.hpp"

#include <iostream>
#include <ostream>

namespace Engine::Renderer {
    OpenGLRenderer::OpenGLRenderer(Window::WindowContext windowContext) {
        if (!gladLoadGLLoader((GLADloadproc) SDL_GL_GetProcAddress)) {
            throw std::runtime_error("Failed to initialize OpenGL context");
        }

        std::cout << "OpenGL version (reported): " << glGetString(GL_VERSION) << std::endl;
        std::cout << "GL Vendor: " << glGetString(GL_VENDOR) << std::endl;
        std::cout << "GL Renderer: " << glGetString(GL_RENDERER) << std::endl;
        std::cout << "GL Shading Language: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;

        m_VAO = 0;
        m_VBO = 0;
        m_shaderProgram = 0;
    }

    OpenGLRenderer::~OpenGLRenderer() = default;

    void OpenGLRenderer::Initialize() {
        m_shaderProgram = LoadShaders();

        std::array<float, 9> vertices = {
            0.0f, 0.5f, 0.0f,
            -0.5f, -0.5f, 0.0f,
            0.5f, -0.5f, 0.0f,
        };

        glGenVertexArrays(1, &m_VAO);
        glGenBuffers(1, &m_VBO);

        glBindVertexArray(m_VAO);
        glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices.data(), GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *) 0);
        glEnableVertexAttribArray(0);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }

    void OpenGLRenderer::Render() {
        glClearColor(0.1f, 0.1f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(m_shaderProgram);
        glBindVertexArray(m_VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);
    }

    void OpenGLRenderer::Shutdown() {
        glDeleteProgram(m_shaderProgram);
        glDeleteBuffers(1, &m_VBO);
        glDeleteVertexArrays(1, &m_VAO);
    }

    unsigned int OpenGLRenderer::LoadShaders() {
        const std::string vertexShaderCode =
                "#version 410 core\nlayout(location = 0) in vec3 aPos;\nvoid main() {\n  gl_Position = vec4(aPos, 1.0);\n}";
        const std::string fragmentShaderCode =
                "#version 410 core\n out vec4 FragColor;\n void main() {\n FragColor= vec4(1.0,0.4,0.2,1.0);\n}";

        const char *vSrc = vertexShaderCode.c_str();
        const char *fSrc = fragmentShaderCode.c_str();

        GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertexShader, 1, &vSrc, nullptr);
        glCompileShader(vertexShader);

        GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentShader, 1, &fSrc, nullptr);
        glCompileShader(fragmentShader);

        GLuint shaderProgram = glCreateProgram();
        glAttachShader(shaderProgram, vertexShader);
        glAttachShader(shaderProgram, fragmentShader);
        glLinkProgram(shaderProgram);

        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);

        return shaderProgram;
    }
} // namespace
