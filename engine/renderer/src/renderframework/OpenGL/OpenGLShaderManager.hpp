#pragma once
#include <optional>
#include <unordered_map>
#include <glad/glad.h>
#include <spdlog/spdlog.h>
#include "../../shadermanagement/Shader.hpp"
#include "../../shadermanagement/ShaderManager.hpp"

namespace Engine::Renderer::RenderFramework::OpenGl {
    class OpenGlShaderManager {
    public:
        OpenGlShaderManager(ShaderManagement::ShaderManager *shader_manager);

        ~OpenGlShaderManager();

        void CompileShaders();

        std::optional<GLuint> GetShaderProgram(const std::string &shader_name) const;

    private:
        ShaderManagement::ShaderManager *m_shader_manager;
        std::unordered_map<std::string, GLuint> m_shader_program_map;


        static GLuint CompileShader(GLenum type, std::string_view source, std::string_view debug_name);

        static GLuint LinkShaderProgram(GLuint vertex_shader, GLuint fragment_shader, std::string_view debug_name);

        static void LogSourceWithLineNumbers(std::string_view source);
    };
} // namespace
