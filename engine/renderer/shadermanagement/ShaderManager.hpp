//
// Created by Godri on 8/10/2025.
//

#pragma once
#include <unordered_map>
#include <filesystem>
#include <optional>

#include "Shader.hpp"

namespace Engine::Renderer::ShaderManagement {
    class ShaderManager {
        public:
        ShaderManager();
        ~ShaderManager();

        void LoadShader(const std::string& shader_name);
        std::optional<Shader> GetShader(const std::string& shader_name);

        std::vector<std::string> GetAllShaderNames();

    private:
        static std::string ReadShaderFile(const std::filesystem::path& filepath);

        std::unordered_map<std::string, Shader> m_shaders;
        const std::string m_shader_path = "shaders";
    };
}
