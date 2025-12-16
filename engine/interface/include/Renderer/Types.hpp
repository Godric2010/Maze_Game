//
// Created by Sebastian Borsch on 16.12.25.
//

#pragma once
#include <cstdint>
#include <vector>
#include <glm/glm.hpp>

namespace Engine::Renderer {
    using MeshHandle = uint32_t;
    using TextureHandle = uint32_t;


    struct MeshVertex {
        glm::vec3 position;
        glm::vec2 uv;
    };

    struct MeshAsset {
        std::vector<MeshVertex> vertices;
        std::vector<uint32_t> indices;
    };

    struct TextureAsset {
        float width;
        float height;
        std::vector<uint8_t> pixels;
    };
}
