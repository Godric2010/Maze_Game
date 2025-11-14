#pragma once
#include "LayoutEngine.hpp"
#include "Types.hpp"

namespace Engine::Text {
    /**
     * @class TextMeshBuilder
     * Build meshes in local space that can be used by the renderer to display text.
     */
    class TextMeshBuilder {
    public:
        TextMeshBuilder() = default;

        ~TextMeshBuilder() = default;

        TextMesh GenerateTextMesh(const TextLayout &layout);
    };
} // namespace
