#include "FontManager.hpp"

namespace Engine::Text {
    bool FontManager::HasFont(const FontHandle &font_handle) const {
        return m_fonts.contains(font_handle);
    }

    void FontManager::LoadFont(const FontHandle &font_handle, const std::vector<uint8_t> &bytes, size_t size) {
        // auto face = std::make_unique<FontFace>(m_font_library, bytes, size, pixel_size);
        // m_fonts.emplace(font_handle, std::move(face));
    }

    Font FontManager::GetFont(const FontHandle &font_handle) const {
        return m_fonts.at(font_handle);
    }

    FontHandle FontManager::GenerateFontHandle(const std::string &name, const int pixel_size) {
        const auto string_hash = std::hash<std::string>()(name);
        const auto pixel_hash = std::hash<int>()(pixel_size);
        const auto font_hash = string_hash ^ pixel_hash;
        return font_hash;
    }
} // namespace
