//
// Created by Godri on 8/10/2025.
//

#pragma once
#include <unordered_map>
#include <filesystem>

#include "Shader.hpp"

namespace Engine::Renderer::ShaderManagement {
	class ShaderManager {
		public:
		ShaderManager();
		~ShaderManager();

		void LoadShader(const std::string& shaderName);
		std::optional<Shader> GetShader(const std::string& shaderName);

	private:
		static std::string readShaderFile(std::filesystem::path filepath);

		std::unordered_map<std::string, Shader> m_shaders;
		const std::string m_shaderPath = "shaders";
	};
}
