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

namespace Engine::Renderer::RenderFramework::OpenGl {


    class OpenGlRenderer final : public IRenderer {
    public:
        explicit OpenGlRenderer(const Environment::WindowContext &window_context, ShaderManagement::ShaderManager *shader_manager);

        ~OpenGlRenderer() override;

        void Initialize() override;

        void PrepareFrame(const CameraAsset& camera_asset) override;

        void DrawFrame(DrawAssets &draw_assets) override;

        MeshHandle AddMesh(const MeshAsset &mesh) override;

        void RemoveMesh(const MeshHandle &mesh_handle) override;

        void Shutdown() override;

    private:
        CameraAsset m_camera_asset{};

        GLuint m_shader_program;
        GLuint m_camera_ubo;
        static constexpr GLuint camera_binding_point = 0;


        ShaderManagement::ShaderManager *m_shader_manager;
        std::unique_ptr<OpenGLMeshManager> m_mesh_manager;

        static void LogSourceWithLineNumbers(std::string_view source);

        [[nodiscard]] GLuint LoadShaders() const;
        static GLuint CompileShader(GLenum type, std::string_view source, std::string_view debug_name);
        static GLuint LinkShaderProgram(GLuint vertex_shader, GLuint fragment_shader, std::string_view debug_name);
    };
} // namespace
