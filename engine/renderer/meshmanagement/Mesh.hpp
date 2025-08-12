#pragma once
#include <vector>
#include <glm/glm.hpp>

namespace Engine::Renderer::Meshmanagement {
    struct Mesh {
        std::vector<glm::vec3> vertices;
        std::vector<uint32_t> indices;
    };
} // namespace
