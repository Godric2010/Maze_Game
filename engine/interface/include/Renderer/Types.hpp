//
// Created by Sebastian Borsch on 16.12.25.
//

#pragma once
#include <cstdint>
#include <vector>
#include <glm/glm.hpp>

namespace Engine::Renderer
{
    using MeshHandle = uint32_t;
    using TextureHandle = uint32_t;


    struct MeshVertex
    {
        glm::vec3 position;
        glm::vec2 uv;
        glm::vec3 normal;
    };

    struct MeshAsset
    {
        std::vector<MeshVertex> vertices;
        std::vector<uint32_t> indices;
    };

    enum class PixelFormat
    {
        R8,
        RGB8,
        RGBA8,
    };

    struct TextureAsset
    {
        uint32_t width;
        uint32_t height;
        PixelFormat format;
        std::vector<uint8_t> pixels;
    };
}
