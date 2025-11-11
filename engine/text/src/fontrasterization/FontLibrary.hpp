#pragma once
#include <ft2build.h>
#include FT_FREETYPE_H
#include <stdexcept>
#include <spdlog/spdlog.h>

namespace Engine::Text::FontRasterization {
    class FontLibrary {
    public:
        FontLibrary() {
            if (FT_Init_FreeType(&m_library)) {
                throw std::runtime_error("Failed to initialize freetype library");
            }
        }

        ~FontLibrary() {
            if (m_library) {
                FT_Done_FreeType(m_library);
            }
        }

        [[nodiscard]] FT_Library Get() const {
            return m_library;
        }

        FontLibrary(const FontLibrary &) = delete;

        FontLibrary(FontLibrary &&) = delete;

        FontLibrary &operator=(const FontLibrary &) = delete;

        FontLibrary &operator=(FontLibrary &&) = delete;

    private:
        FT_Library m_library = nullptr;
    };
} // namespace
