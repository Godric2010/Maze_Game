#pragma once
#include <optional>
#include <unordered_map>
#include <glad/glad.h>
#include <spdlog/spdlog.h>

#include "AssetHandler.hpp"

namespace Engine::Renderer::RenderFramework::OpenGl {
    class OpenGlShaderManager {
    public:
        OpenGlShaderManager(AssetHandling::AssetHandler* asset_handler);

        ~OpenGlShaderManager();

        void CompileShaders();

        std::optional<GLuint> GetShaderProgram(const std::string& shader_name) const;

    private:
        AssetHandling::AssetHandler* m_asset_handler;
        std::unordered_map<std::string, GLuint> m_shader_program_map;


        static GLuint CompileShader(GLenum type, std::string_view source, std::string_view debug_name);

        static GLuint LinkShaderProgram(GLuint vertex_shader, GLuint fragment_shader, std::string_view debug_name);

        static void LogSourceWithLineNumbers(std::string_view source);
    };
} // namespace
