#pragma once
#include <string>

namespace Engine::Renderer::ShaderManagement {
    struct Shader {
        std::string name;
        std::string vertexShader;
        std::string fragmentShader;
    };
} // namespace
