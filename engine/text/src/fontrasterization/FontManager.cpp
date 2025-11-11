#include "FontManager.hpp"

namespace Engine::Text::FontRasterization {
    FontManager::FontManager(FontLibrary &library) : m_font_library(library) {
    }

    FontHandle FontManager::LoadFont(const std::vector<uint8_t> &bytes, size_t size, const std::string &name,
                                     int pixel_size) {
        uint32_t id = ++m_next_id;
        auto face = std::make_unique<FontFace>(m_font_library, bytes, size, pixel_size);
        m_fonts.emplace(id, std::move(face));

        FontHandle handle;
        handle.font_name = name;
        handle.font_size = pixel_size;
        handle.id = id;
        return handle;
    }
} // namespace
