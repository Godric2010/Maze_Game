#pragma once
#include "FontManager.hpp"

namespace Engine::Text {
    /**
     * @struct GlyphData
     * Contains all data needed to define the layout of a glyph
     */
    struct GlyphData {
        float x0, y0; // top-left
        float x1, y1; // bottom-right
        float u0, v0; // top-left UV
        float u1, v1; // bottom-right UV
    };

    /**
     * @struct TextLayout
     * Layout data that is used to define the layout of a text, based on the glyph information.
     */
    struct TextLayout {
        std::vector<GlyphData> glyph_data;

        float min_x = 0.0f, max_x = 0.0f, min_y = 0.0f, max_y = 0.0f;

        [[nodiscard]] float Width() const { return max_x - min_x; }
        [[nodiscard]] float Height() const { return max_y - min_y; }
    };

    /**
     * @class LayoutEngine
     * Layouting texts and create text layout for further usage.
     */
    class LayoutEngine {
    public:
        LayoutEngine() = default;

        ~LayoutEngine() = default;

        TextLayout GenerateTextLayout(const Font &font, const std::vector<uint32_t> &codepoints,
                                      const TextAlignment &alignment);

    private:
    };
} // namespace
