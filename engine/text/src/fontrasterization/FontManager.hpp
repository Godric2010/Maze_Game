#pragma once
#include <unordered_map>

#include "FontFace.hpp"
#include "FontHandle.hpp"
#include "FontLibrary.hpp"

namespace Engine::Text::FontRasterization {
    class FontManager {
    public:
        explicit FontManager(FontLibrary &library);

        ~FontManager() = default;

        FontHandle LoadFont(const std::vector<uint8_t> &bytes, size_t size, const std::string &name,
                            int pixel_size);

        FontFace &Get(const FontHandle &font_handle);

    private:
        FontLibrary &m_font_library;
        std::unordered_map<uint32_t, std::unique_ptr<FontFace> > m_fonts;
        uint32_t m_next_id = 0;
    };
} // namespace
