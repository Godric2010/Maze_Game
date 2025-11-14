#pragma once
#include "FontManager.hpp"

namespace Engine::Text {
    /**
     * @struct GlyphData
     * Contains all data needed to define the layout of a glyph
     */
    struct GlyphData {
        float position_x, position_y;
        float width, height;
        float texture_u, texture_v;
    };

    /**
     * @struct TextLayout
     * Layout data that is used to define the layout of a text, based on the glyph information.
     */
    struct TextLayout {
        std::vector<GlyphData> glyph_data;
        float bounding_box_width;
        float bounding_box_height;
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
