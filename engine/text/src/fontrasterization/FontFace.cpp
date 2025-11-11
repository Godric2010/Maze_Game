#include "FontFace.hpp"

#include <spdlog/spdlog.h>

namespace Engine::Text::FontRasterization {
    FontFace::FontFace(const FontLibrary &font_library, const std::vector<uint8_t> &bytes, const size_t size,
                       const int pixel_size) : m_font_size(pixel_size) {
        if (bytes.empty() || size == 0) {
            spdlog::critical("Failed to load font data!");
            throw std::runtime_error("Failed to load font data!");
        }
        m_font_data = bytes;
        const auto base = reinterpret_cast<const FT_Byte *>(m_font_data.data());
        const auto font_lib = font_library.Get();
        const auto ft_size = static_cast<FT_Long>(size);
        FT_Error error = FT_New_Memory_Face(font_lib, base, ft_size, 0, &m_face);

        if (error) {
            spdlog::error("Failed to load font library. Error: {}", error);
            throw std::runtime_error("Failed to load font data");
        }

        error = FT_Set_Pixel_Sizes(m_face, 0, pixel_size);
        if (error) {
            spdlog::error("Failed to set pixel sizes in font library. Error: {}", error);
            throw std::runtime_error("Failed to set pixel sized in font library.");
        }

        m_ascender = static_cast<float>(m_face->size->metrics.ascender) / 64.0f;
        m_descender = static_cast<float>(m_face->size->metrics.descender) / 64.0f;
        m_line_height = static_cast<float>(m_face->size->metrics.height) / 64.0f;
    }

    FontFace::~FontFace() {
        if (m_face) {
            FT_Done_Face(m_face);
        }
    }

    bool FontFace::GetKerning(const uint32_t left, const uint32_t right, float &out_advance) const {
        if (!HasKerning()) {
            return false;
        }
        FT_Vector delta{};
        if (FT_Get_Kerning(m_face, left, right, FT_KERNING_DEFAULT, &delta)) {
            return false;
        }
        out_advance = static_cast<float>(delta.x) / 64.0f;
        return true;
    }

    GlyphBitmap FontFace::Rasterize(uint32_t codepoint) {
        if (FT_Load_Char(m_face, codepoint, FT_LOAD_RENDER)) {
            spdlog::error("Failed to load glyph '{}'", codepoint);
            throw std::runtime_error("Failed to load glyph");
        }

        FT_GlyphSlot glyph = m_face->glyph;
        GlyphBitmap out{};
        out.width = glyph->bitmap.width;
        out.height = glyph->bitmap.rows;
        out.bearing_x = glyph->bitmap_left;
        out.bearing_y = glyph->bitmap_top;
        out.advance = static_cast<int>(glyph->advance.x >> 6);
        out.pixels.resize(out.width * out.height);

        const uint8_t *src = glyph->bitmap.buffer;
        for (int y = 0; y < out.height; ++y) {
            std::memcpy(out.pixels.data() + y * out.width, src + y * glyph->bitmap.pitch, out.width);
        }
        return out;
    }
} // namespace
