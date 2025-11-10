//
// Created by Sebastian Borsch on 09.11.25.
//

#pragma once
#include <cstdint>
#include <vector>

namespace Engine::Text {
    struct GlyphVertex {
        float x, y;
        float u, v;
        uint32_t color;
    };

    struct TextMeshHandle {
        uint32_t atlas_id;
        std::vector<GlyphVertex> vertices;
        std::vector<uint32_t> indices;
    };
}
