//
// Created by Sebastian Borsch on 13.11.25.
//

#pragma once
#include <vector>

namespace Engine::Text {
    /**
     * @typedef FontHandle
     * uint32_t representing a hash value from the fonts name and its size in pixels.
     */
    using FontHandle = uint32_t;

    struct FontHandleResult {
        FontHandle font_handle;
        bool new_atlas_created;
    };

    /**
     * @enum TextAlignment
     * Defines the alignment of a text horizontally.
     */
    enum class TextAlignment {
        Left,
        Center,
        Right
    };

    /**
     * @struct TextMeshVertex
     * Defines a vertex for the text mesh, with its position and uv coordinates.
     */
    struct TextMeshVertex {
        float x;
        float y;
        float u;
        float v;
    };

    /**
     * @struct TextMesh
     * Struct containing all necessary information to build a mesh for the text in local space.
     */
    struct TextMesh {
        std::vector<TextMeshVertex> vertices;
        std::vector<uint32_t> indices;
    };

    /**
     * @struct FontTextureDescriptor
     * Describes the plain data to build a texture for rendering a font.
     */
    struct FontTextureDescriptor {
        uint32_t width, height;
        std::vector<uint8_t> pixels;
    };
}
