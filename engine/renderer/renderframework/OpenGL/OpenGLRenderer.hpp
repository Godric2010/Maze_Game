#pragma once
#include "Window.hpp"
#include "../Renderer.hpp"
#include <glad/glad.h>
#include <array>
#include <string>
#include <string_view>
#include <ostream>
#include <sstream>
#include <stdexcept>
#include <spdlog/spdlog.h>

#include "../../shadermanagement/ShaderManager.hpp"

namespace Engine::Renderer::RenderFramework::OpenGL {
    struct OpenGLMesh {
        unsigned int VAO;
        unsigned int VBO;
        unsigned int EBO;
        size_t numVertices;
        size_t numIndices;
    };

    class OpenGLRenderer final : public IRenderer {
    public:
        explicit OpenGLRenderer(Window::WindowContext windowContext, ShaderManagement::ShaderManager *shaderManager);

        ~OpenGLRenderer() override;

        void Initialize() override;

        void PrepareFrame(glm::mat4 camView, glm::mat4 camProj, glm::vec3 camPos) override;

        void DrawFrame() override;

        void AddMesh(const Meshmanagement::Mesh &mesh) override;

        void RemoveMesh() override;

        void Shutdown() override;

    private:
        std::vector<OpenGLMesh> m_meshes;
        GLuint m_shaderProgram;

        GLuint m_cameraUBO;
        static constexpr GLuint CAMERA_BINDING_POINT = 0;
        struct CameraGPUData {
            glm::mat4 camView;
            glm::mat4 camProj;
            glm::vec4 camPos;
        };

        ShaderManagement::ShaderManager *m_shaderManager;

        static void logSourceWithLineNumbers(std::string_view source);

        [[nodiscard]] GLuint LoadShaders() const;
        static GLuint CompileShader(GLenum type, std::string_view source, std::string_view debugName);
        static GLuint LinkShaderProgram(GLuint vertexShader, GLuint fragmentShader, std::string_view debugName);
    };
} // namespace
