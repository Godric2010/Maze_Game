//
// Created by Godri on 8/10/2025.
//

#include "ShaderManager.hpp"

#include <filesystem>
#include <ranges>
#include <vector>
#include <spdlog/spdlog.h>

namespace Engine::Renderer::ShaderManagement {
    ShaderManager::ShaderManager() {
        m_shaders.clear();
    }

    ShaderManager::~ShaderManager() {
        m_shaders.clear();
    }

    void ShaderManager::LoadShader(const std::string &shader_name) {
        if (m_shaders.contains(shader_name)) {
            return;
        }
        spdlog::info("Loading shader {}", shader_name);

        const auto vert_shader_content = Environment::FileReader::ReadTextContentFromFile(
            "shaders/" + shader_name + ".vert");
        if (!vert_shader_content.Ok()) {
            spdlog::error("Failed to load vertex shader {}\nError: {}", shader_name, vert_shader_content.error.message);
            throw std::runtime_error("Failed to load vertex shader" + shader_name);
        }
        const auto frag_shader_content = Environment::FileReader::ReadTextContentFromFile(
            "shaders/" + shader_name + ".frag");
        if (!frag_shader_content.Ok()) {
            spdlog::error("Failed to load fragment shader {}\nError: {}", shader_name,
                          frag_shader_content.error.message);
            throw std::runtime_error("Failed to load fragment shader" + shader_name);
        }

        const auto shader = Shader{
            .name = shader_name,
            .vertex_shader = vert_shader_content.value,
            .fragment_shader = frag_shader_content.value,
        };
        m_shaders[shader_name] = shader;
    }

    std::optional<Shader> ShaderManager::GetShader(const std::string &shader_name) {
        if (const auto it = m_shaders.find(shader_name); it != m_shaders.end()) {
            return std::make_optional(it->second);
        }
        return std::nullopt;
    }

    std::vector<std::string> ShaderManager::GetAllShaderNames() {
        std::vector<std::string> names;
        for (const auto &shader_name: m_shaders | std::views::keys) {
            names.push_back(shader_name);
        }
        return names;
    }
}
