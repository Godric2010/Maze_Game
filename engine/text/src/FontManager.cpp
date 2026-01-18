#include "FontManager.hpp"
#include <functional>

#include <ft2build.h>
#include FT_FREETYPE_H

#include <algorithm>
#include <stdexcept>
#include <spdlog/spdlog.h>

namespace Engine::Text {
    FontManager::FontManager(AssetHandling::AssetHandler* asset_handler) {
        m_asset_handler = asset_handler;
    }

    bool FontManager::HasFont(const FontHandle& font_handle) const {
        return m_fonts.contains(font_handle);
    }

    FontHandleResult FontManager::LoadFont(const std::string& font_name, const int pixel_size) {
        auto font_handle = GenerateFontHandle(font_name, pixel_size);
        if (HasFont(font_handle)) {
            return {font_handle, false};
        }

        auto font = BuildFont(font_name, pixel_size);
        m_fonts.emplace(font_handle, std::move(font));
        return {font_handle, true};
    }

    Font FontManager::GetFont(const FontHandle& font_handle) const {
        return m_fonts.at(font_handle);
    }

    Font FontManager::BuildFont(const std::string& font_name, const int pixel_size) const {
        auto font_asset = m_asset_handler->LoadAssetWithoutCaching<AssetHandling::FontAsset>(font_name);
        auto font_bytes = font_asset->bytes;

        FT_Library ft_library = nullptr;
        if (FT_Init_FreeType(&ft_library) != 0) {
            spdlog::error("Failed to initialize freetype library.");
            throw std::runtime_error("Failed to initialize freetype library.");
        }
        FT_Face face = nullptr;
        if (FT_New_Memory_Face(ft_library, font_bytes.data(), static_cast<FT_Long>(font_bytes.size()), 0, &face) != 0) {
            FT_Done_FreeType(ft_library);
            spdlog::error("Failed to build FT_Face from binary data.");
            throw std::runtime_error("Failed to build FT_Face from binary data.");
        }

        if (FT_Set_Pixel_Sizes(face, 0, pixel_size)) {
            FT_Done_Face(face);
            FT_Done_FreeType(ft_library);
            spdlog::error("Failed to set pixel size to size {} for font {}", pixel_size, font_name);
            throw std::runtime_error("Failed to set pixel size to size for font");
        }

        Font font{};
        font.name = font_name;
        font.pixel_size = pixel_size;
        font.ascender = face->size->metrics.ascender >> 6; // 26.6 fixed point -> >> 6
        font.descender = face->size->metrics.descender >> 6;
        font.line_height = face->size->metrics.height >> 6;

        struct GlyphBitmapTemp {
            std::uint32_t codepoint;
            unsigned int width;
            unsigned int height;
            int bearing_x;
            int bearing_y;
            int advance;
            std::vector<std::uint8_t> bitmap;
        };

        std::vector<GlyphBitmapTemp> glyph_bitmaps;
        glyph_bitmaps.reserve(126 - 32 + 1);

        unsigned int max_row_width = 0;
        unsigned int current_row_width = 0;
        unsigned int current_row_height = 0;
        unsigned int total_height = 0;

        for (uint32_t cp = 32; cp <= 126; ++cp) {
            if (FT_Load_Char(face, static_cast<FT_ULong>(cp), FT_LOAD_RENDER) != 0) {
                // Glyph does not exist in font
                continue;
            }
            FT_GlyphSlot glyph = face->glyph;
            FT_Bitmap glyph_bitmap = glyph->bitmap;

            GlyphBitmapTemp temp{};
            temp.codepoint = cp;
            temp.width = glyph->bitmap.width;
            temp.height = glyph->bitmap.rows;
            temp.bearing_x = glyph->bitmap_left;
            temp.bearing_y = glyph->bitmap_top;
            temp.advance = glyph->advance.x >> 6;

            temp.bitmap.resize(temp.width * temp.height);
            for (int y = 0; y < glyph_bitmap.rows; ++y) {
                const auto* src = glyph_bitmap.buffer + y * glyph_bitmap.pitch;
                auto* dst = temp.bitmap.data() + y * glyph_bitmap.width;
                std::copy_n(src, glyph_bitmap.width, dst);
            }
            if (current_row_width + temp.width > 1024) {
                max_row_width = std::max(max_row_width, current_row_width);
                total_height += current_row_height;
                current_row_width = 0;
                current_row_height = 0;
            }
            current_row_width += temp.width;
            current_row_height = std::max(current_row_height, temp.height);
            glyph_bitmaps.push_back(std::move(temp));
        }

        max_row_width = std::max(max_row_width, current_row_width);
        total_height += current_row_height;

        if (glyph_bitmaps.empty()) {
            FT_Done_Face(face);
            FT_Done_FreeType(ft_library);
            spdlog::error("No glyphs in font!");
            throw std::runtime_error("No glyphs in font!");
        }

        font.atlas_width = max_row_width;
        font.atlas_height = total_height;
        font.atlas_pixels.assign(font.atlas_width * font.atlas_height, 0);

        unsigned int pen_x = 0;
        unsigned int pen_y = 0;
        unsigned int row_height = 0;
        for (const auto& glyph_bitmap: glyph_bitmaps) {
            if (pen_x + glyph_bitmap.width > font.atlas_width) {
                pen_x = 0;
                pen_y += row_height;
                row_height = 0;
            }
            for (int y = 0; y < glyph_bitmap.height; ++y) {
                unsigned int dest_y = pen_y + y;
                if (dest_y > font.atlas_height) {
                    continue;
                }
                uint8_t* dest_row = font.atlas_pixels.data() + dest_y * font.atlas_width;
                const uint8_t* src_row = glyph_bitmap.bitmap.data() + y * glyph_bitmap.width;
                std::copy_n(src_row, glyph_bitmap.width, dest_row + pen_x);
            }

            float u0 = static_cast<float>(pen_x) / static_cast<float>(font.atlas_width);
            float v0 = static_cast<float>(pen_y) / static_cast<float>(font.atlas_height);
            float u1 = static_cast<float>(pen_x + glyph_bitmap.width) / static_cast<float>(font.atlas_width);
            float v1 = static_cast<float>(pen_y + glyph_bitmap.height) / static_cast<float>(font.atlas_height);

            GlyphMetrics metrics{};
            metrics.width = glyph_bitmap.width;
            metrics.height = glyph_bitmap.height;
            metrics.bearing_x = glyph_bitmap.bearing_x;
            metrics.bearing_y = glyph_bitmap.bearing_y;
            metrics.advance_x = glyph_bitmap.advance;
            metrics.u0 = u0;
            metrics.v0 = v0;
            metrics.u1 = u1;
            metrics.v1 = v1;

            font.glyphs.emplace(glyph_bitmap.codepoint, metrics);

            pen_x += glyph_bitmap.width;
            row_height = std::max(row_height, glyph_bitmap.height);
        }

        FT_Done_Face(face);
        FT_Done_FreeType(ft_library);

        return font;
    }

    FontHandle FontManager::GenerateFontHandle(const std::string& name, const int pixel_size) {
        const auto string_hash = std::hash<std::string>()(name);
        const auto pixel_hash = std::hash<int>()(pixel_size);
        const auto font_hash = string_hash ^ pixel_hash;
        return font_hash;
    }
} // namespace
