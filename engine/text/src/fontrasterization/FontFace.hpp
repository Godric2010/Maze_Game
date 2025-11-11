#pragma once
#include <ft2build.h>
#include FT_FREETYPE_H
#include <memory>
#include <string>
#include <cstdint>
#include <stdexcept>
#include <vector>

#include "FontLibrary.hpp"

namespace Engine::Text::FontRasterization {
    struct GlyphBitmap {
        int width;
        int height;
        int bearing_x;
        int bearing_y;
        int advance;
        std::vector<uint8_t> pixels;
    };


    class FontFace {
    public:
        FontFace(const FontLibrary &font_library, const std::vector<uint8_t> &bytes, size_t size, int pixel_size);

        ~FontFace();

        [[nodiscard]] int SizeInPixels() const { return m_font_size; }
        [[nodiscard]] float Ascender() const { return m_ascender; }
        [[nodiscard]] float Descender() const { return m_descender; }
        [[nodiscard]] float LineHeight() const { return m_line_height; }

        [[nodiscard]] bool HasKerning() const { return FT_HAS_KERNING(m_face); }

        bool GetKerning(uint32_t left, uint32_t right, float &out_advance) const;

        GlyphBitmap Rasterize(uint32_t codepoint);

    private:
        FT_Face m_face = nullptr;
        std::vector<uint8_t> m_font_data;
        int m_font_size = 0;
        float m_ascender = 0.f;
        float m_descender = 0.f;
        float m_line_height = 0.f;
    };
} // namespace
