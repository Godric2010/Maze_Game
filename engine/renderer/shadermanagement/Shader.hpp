#pragma once
#include <string>

namespace Engine::Renderer::ShaderManagement {
    struct Shader {
        std::string name;
        std::string vertex_shader;
        std::string fragment_shader;
    };
} // namespace
