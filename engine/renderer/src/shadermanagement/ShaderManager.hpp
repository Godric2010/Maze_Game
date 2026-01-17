//
// Created by Godri on 8/10/2025.
//

#pragma once
#include <unordered_map>
#include <optional>
#include <vector>
#include "IFileReader.hpp"

#include "Shader.hpp"

namespace Engine::Renderer::ShaderManagement {
    class ShaderManager {
    public:
        explicit ShaderManager(Environment::Files::IFileReader* file_reader);

        ~ShaderManager();

        void LoadShader(const std::string& shader_name);

        std::optional<Shader> GetShader(const std::string& shader_name);

        std::vector<std::string> GetAllShaderNames();

    private:
        std::unordered_map<std::string, Shader> m_shaders;
        const std::string m_shader_path = "shaders";
        Environment::Files::IFileReader* m_file_reader;
    };
}
