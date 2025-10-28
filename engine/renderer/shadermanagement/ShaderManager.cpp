//
// Created by Godri on 8/10/2025.
//

#include "ShaderManager.hpp"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <ranges>
#include <vector>

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

        std::filesystem::path resource_path = std::filesystem::current_path() / "resources/shaders";
        std::string vertex_path = (resource_path / (shader_name + ".vert")).string();
        std::string fragment_path = (resource_path / (shader_name + ".frag")).string();

        std::cout << "Loading shader " << shader_name << "..." << std::endl;
        std::cout << vertex_path << std::endl;
        std::cout << fragment_path << std::endl;

        const auto vertex_shader = ReadShaderFile(vertex_path);
        const auto fragment_shader = ReadShaderFile(fragment_path);

        const auto shader = Shader{
            .name = shader_name,
            .vertex_shader = vertex_shader,
            .fragment_shader = fragment_shader
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

    std::string ShaderManager::ReadShaderFile(const std::filesystem::path &filepath) {
        std::ifstream file(filepath, std::ios::ate | std::ios::binary);
        if (!file.is_open()) {
            throw std::runtime_error("Could not open file " + filepath.string());
        }
        size_t file_size = (size_t) file.tellg();
        std::vector<char> buffer(file_size);
        file.seekg(0);
        file.read(buffer.data(), file_size);
        file.close();
        return {buffer.data(), file_size};
    }
}
