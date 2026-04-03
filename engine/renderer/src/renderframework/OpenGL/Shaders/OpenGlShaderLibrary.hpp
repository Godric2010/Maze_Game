#pragma once
#include <optional>
#include <unordered_map>
#include <GL/glew.h>
#include <spdlog/spdlog.h>

#include "AssetHandler.hpp"
#include "../../../resources/IShaderLibrary.hpp"

namespace Engine::Renderer::RenderFramework::OpenGl
{
    class OpenGlShaderLibrary : public Resources::IShaderLibrary
    {
    public:
        OpenGlShaderLibrary(AssetHandling::AssetHandler* asset_handler);

        ~OpenGlShaderLibrary() override;

        void CompileShaders(
            const std::vector<std::tuple<Assets::ShaderHandle, std::shared_ptr<const AssetHandling::ShaderAsset>>>& shaders) override;

        void ClearShaders() override;

        void CompileShaders();

        std::optional<GLuint> GetShaderProgram(const Assets::ShaderHandle& shader_handle) const;

    private:
        AssetHandling::AssetHandler* m_asset_handler;
        std::unordered_map<size_t, GLuint> m_shader_program_map;


        static GLuint CompileShader(GLenum type, std::string_view source, std::string_view debug_name);

        static GLuint LinkShaderProgram(GLuint vertex_shader, GLuint fragment_shader, std::string_view debug_name);

        static void LogSourceWithLineNumbers(std::string_view source);
    };
} // namespace
