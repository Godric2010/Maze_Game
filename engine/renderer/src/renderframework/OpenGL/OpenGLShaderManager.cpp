#include "OpenGLShaderManager.hpp"

#include <ranges>


namespace Engine::Renderer::RenderFramework::OpenGl {
    OpenGlShaderManager::OpenGlShaderManager(AssetHandling::AssetHandler* asset_handler) {
        m_asset_handler = asset_handler;
    }

    OpenGlShaderManager::~OpenGlShaderManager() {
        for (const auto shader: m_shader_program_map | std::views::values) {
            glDeleteShader(shader);
        }

        m_asset_handler = nullptr;
    }

    void OpenGlShaderManager::CompileShaders() {
        const auto shader_assets = m_asset_handler->GetAllAssetsOfType<AssetHandling::ShaderAsset>();

        for (const auto& shader_asset: shader_assets) {
            const std::string shader_name = shader_asset->name;
            const char* v_src = shader_asset->vertex_content.c_str();
            const char* f_src = shader_asset->fragment_content.c_str();

            const auto vertex_shader = CompileShader(GL_VERTEX_SHADER, v_src, shader_name + ".vertexShader");
            const auto fragment_shader = CompileShader(GL_FRAGMENT_SHADER, f_src, shader_name + ".fragmentShader");

            const auto shader_program = LinkShaderProgram(vertex_shader, fragment_shader, shader_name);

            m_shader_program_map.emplace(shader_name, shader_program);
        }
    }

    std::optional<GLuint> OpenGlShaderManager::GetShaderProgram(const std::string& shader_name) const {
        if (!m_shader_program_map.contains(shader_name)) {
            return std::nullopt;
        }
        return m_shader_program_map.at(shader_name);
    }

    GLuint OpenGlShaderManager::CompileShader(const GLenum type, const std::string_view source,
                                              const std::string_view debug_name) {
        const GLuint shader = glCreateShader(type);
        const char* ptr = source.data();
        const auto length = static_cast<GLint>(source.size());
        glShaderSource(shader, 1, &ptr, &length);
        glCompileShader(shader);

        GLint status = GL_FALSE;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &status);

        if (status != GL_TRUE) {
            GLint log_length = 0;
            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &log_length);
            const std::string log(log_length > 1 ? log_length - 1 : 0, '\0');
            spdlog::error("[Shader Compile] {} failed: \n{}", std::string(debug_name).c_str(), log.c_str());
            LogSourceWithLineNumbers(source);

            glDeleteShader(shader);
            throw std::runtime_error("Failed to compile shader " + std::string(debug_name));
        }
        return shader;
    }

    GLuint OpenGlShaderManager::LinkShaderProgram(const GLuint vertex_shader, const GLuint fragment_shader,
                                                  const std::string_view debug_name) {
        const GLuint program = glCreateProgram();
        glAttachShader(program, vertex_shader);
        glAttachShader(program, fragment_shader);
        glLinkProgram(program);

        GLint status = GL_FALSE;
        glGetProgramiv(program, GL_LINK_STATUS, &status);

        if (status != GL_TRUE) {
            GLint log_length = 0;
            glGetProgramiv(program, GL_INFO_LOG_LENGTH, &log_length);
            std::string log(log_length > 1 ? log_length - 1 : 0, '\0');
            if (log_length > 1) {
                glGetProgramInfoLog(program, log_length, nullptr, reinterpret_cast<GLchar*>(log.data()));
            }
            spdlog::error("[Program Link] {} failed: \n{}", std::string(debug_name).c_str(), log.c_str());

            glDeleteProgram(program);
            throw std::runtime_error("Failed to link shader program " + std::string(debug_name));
        }

        glDetachShader(program, vertex_shader);
        glDetachShader(program, fragment_shader);
        glDeleteShader(vertex_shader);
        glDeleteShader(fragment_shader);

        return program;
    }

    void OpenGlShaderManager::LogSourceWithLineNumbers(const std::string_view source) {
        std::istringstream iss{std::string(source)};
        std::string line;
        int line_number = 1;
        while (std::getline(iss, line)) {
            spdlog::info("{:4d}: {}", line_number++, line);
        }
    }
} // namespace
