#pragma once
#include <unordered_map>
#include "string"

#include <AssetHandler.hpp>
#include "Types.hpp"

namespace Engine::Text {
    /**
     * @struct GlyphMetrics
     * Contains the metrics for one glyph, loaded from the freetype library out of the font-file
     */
    struct GlyphMetrics {
        int width;
        int height;
        int bearing_x;
        int bearing_y;
        int advance_x;

        float u0, v0; // UV top-left in atlas
        float u1, v1; // UV bottom-right in atlas
    };

    /**
     * @struct Font
     * Holds all data relevant to use a font for text rendering.
     */
    struct Font {
        std::string name;
        unsigned int pixel_size;
        unsigned int atlas_width;
        unsigned int atlas_height;
        std::vector<uint8_t> atlas_pixels;
        std::unordered_map<uint32_t, GlyphMetrics> glyphs;

        int ascender;
        int descender;
        int line_height;

        [[nodiscard]] FontTextureDescriptor GetTextureDescriptor() const {
            FontTextureDescriptor descriptor{};
            descriptor.width = atlas_width;
            descriptor.height = atlas_height;
            descriptor.pixels = atlas_pixels;
            return descriptor;
        }

        [[nodiscard]] const GlyphMetrics* GetGlyphMetrics(const uint32_t codepoint) const {
            const auto it = glyphs.find(codepoint);
            if (it == glyphs.end()) {
                return nullptr;
            }
            return &it->second;
        }
    };

    /**
     * @class FontManager
     * Is responsible for creating, caching and managing fonts.
     */
    class FontManager {
    public:
        explicit FontManager(AssetHandling::AssetHandler* asset_handler);

        ~FontManager() = default;


        [[nodiscard]] bool HasFont(const FontHandle& font_handle) const;

        FontHandleResult LoadFont(const std::string& font_name, int pixel_size);

        [[nodiscard]] Font GetFont(const FontHandle& font_handle) const;

    private:
        std::unordered_map<FontHandle, Font> m_fonts;
        AssetHandling::AssetHandler* m_asset_handler;

        static FontHandle GenerateFontHandle(const std::string& name, int pixel_size);

        [[nodiscard]] Font BuildFont(const std::string& font_name, int pixel_size) const;
    };
} // namespace
