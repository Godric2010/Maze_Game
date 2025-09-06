//
// Created by Godri on 8/10/2025.
//

#include "ShaderManager.hpp"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <vector>

namespace Engine::Renderer::ShaderManagement {
    ShaderManager::ShaderManager() {
        m_shaders.clear();
    }

    ShaderManager::~ShaderManager() {
        m_shaders.clear();
    }

    void ShaderManager::LoadShader(const std::string &shaderName) {
        if (m_shaders.contains(shaderName)) {
            return;
        }

        std::filesystem::path resourcePath = std::filesystem::current_path() / "resources/shaders";
        std::string vertexPath = (resourcePath / (shaderName + ".vert")).string();
        std::string fragmentPath = (resourcePath / (shaderName + ".frag")).string();

        std::cout << "Loading shader " << shaderName << "..." << std::endl;
        std::cout << vertexPath << std::endl;
        std::cout << fragmentPath << std::endl;

        const auto vertexShader = readShaderFile(vertexPath);
        const auto fragmentShader = readShaderFile(fragmentPath);

        const auto shader = Shader{
            .name = shaderName,
            .vertexShader = vertexShader,
            .fragmentShader = fragmentShader
        };
        m_shaders[shaderName] = shader;
    }

    std::optional<Shader> ShaderManager::GetShader(const std::string &shaderName) {
        if (const auto it = m_shaders.find(shaderName); it != m_shaders.end()) {
            return std::make_optional(it->second);
        }
        return std::nullopt;
    }

    std::string ShaderManager::readShaderFile(const std::filesystem::path filepath) {
        std::ifstream file(filepath, std::ios::ate | std::ios::binary);
        if (!file.is_open()) {
            throw std::runtime_error("Could not open file " + filepath.string());
        }
        size_t fileSize = (size_t) file.tellg();
        std::vector<char> buffer(fileSize);
        file.seekg(0);
        file.read(buffer.data(), fileSize);
        file.close();
        return {buffer.data(), fileSize};
    }
}
