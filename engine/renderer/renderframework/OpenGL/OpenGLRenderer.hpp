#pragma once
#include "Window.hpp"
#include "../Renderer.hpp"
#include <glad/glad.h>
#include <string_view>
#include <spdlog/spdlog.h>
#include <glm/gtc/type_ptr.hpp>

#include "OpenGLMesh.hpp"
#include "OpenGLMeshManager.hpp"
#include "../../shadermanagement/ShaderManager.hpp"

namespace Engine::Renderer::RenderFramework::OpenGL {


    class OpenGLRenderer final : public IRenderer {
    public:
        explicit OpenGLRenderer(Window::WindowContext windowContext, ShaderManagement::ShaderManager *shaderManager);

        ~OpenGLRenderer() override;

        void Initialize() override;

        void PrepareFrame(const CameraAsset& cameraAsset) override;

        void DrawFrame(const std::vector<DrawAsset> &drawAssets) override;

        MeshHandle AddMesh(const MeshAsset &mesh) override;

        void RemoveMesh(const MeshHandle &meshHandle) override;

        void Shutdown() override;

    private:
        CameraAsset m_cameraAsset;

        GLuint m_shaderProgram;
        GLuint m_cameraUBO;
        static constexpr GLuint CAMERA_BINDING_POINT = 0;


        ShaderManagement::ShaderManager *m_shaderManager;
        std::unique_ptr<OpenGLMeshManager> m_meshManager;

        static void logSourceWithLineNumbers(std::string_view source);

        [[nodiscard]] GLuint LoadShaders() const;
        static GLuint CompileShader(GLenum type, std::string_view source, std::string_view debugName);
        static GLuint LinkShaderProgram(GLuint vertexShader, GLuint fragmentShader, std::string_view debugName);
    };
} // namespace
