#pragma once
#include <unordered_map>

#include "FontLibrary.hpp"
#include "Types.hpp"

namespace Engine::Text {
    /**
     * @struct Font
     * Holds all data relevant to use a font for text rendering.
     */
    struct Font {
        [[nodiscard]] FontTextureDescriptor GetTextureDescriptor() const {
            return {};
        }
    };

    /**
     * @class FontManager
     * Is responsible for creating, caching and managing fonts.
     */
    class FontManager {
    public:
        FontManager() = default;

        ~FontManager() = default;

        static FontHandle GenerateFontHandle(const std::string &name, int pixel_size);

        [[nodiscard]] bool HasFont(const FontHandle &font_handle) const;

        void LoadFont(const FontHandle &font_handle, const std::vector<uint8_t> &bytes, size_t size);

        [[nodiscard]] Font GetFont(const FontHandle &font_handle) const;

    private:
        // FontLibrary m_font_library{};
        std::unordered_map<FontHandle, Font> m_fonts;
    };
} // namespace
