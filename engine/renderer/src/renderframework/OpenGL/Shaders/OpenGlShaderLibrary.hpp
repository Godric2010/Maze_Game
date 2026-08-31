#pragma once
#include <optional>
#include <unordered_map>
#include <GL/glew.h>
#include <spdlog/spdlog.h>

#include "AssetHandler.hpp"
#include "../../../resources/IShaderLibrary.hpp"

namespace yarep::renderer::render_framework::open_gl
{
    class OpenGlShaderLibrary : public resources::IShaderLibrary
    {
    public:
        OpenGlShaderLibrary(asset_handling::AssetHandler* asset_handler);

        ~OpenGlShaderLibrary() override;

        void CompileShaders(
            const std::vector<std::tuple<assets::ShaderHandle, std::shared_ptr<const asset_handling::ShaderAsset>>>& shaders) override;

        void ClearShaders() override;

        void CompileShaders();

        std::optional<GLuint> GetShaderProgram(const assets::ShaderHandle& shader_handle) const;

    private:
        asset_handling::AssetHandler* m_asset_handler;
        std::unordered_map<size_t, GLuint> m_shader_program_map;


        static GLuint CompileShader(GLenum type, std::string_view source, std::string_view debug_name);

        static GLuint LinkShaderProgram(GLuint vertex_shader, GLuint fragment_shader, std::string_view debug_name);

        static void LogSourceWithLineNumbers(std::string_view source);
    };
} // namespace
