#pragma once
#include "../src/fontrasterization/FontLibrary.hpp"
#include "../src/fontrasterization/FontManager.hpp"

namespace Engine::Text {
    class TextController{
    public:
        TextController();

        ~TextController() = default;

        [[nodiscard]] FontHandle LoadFont(std::string font_name, int pixel_size) const;

    private:
        FontRasterization::FontLibrary m_font_library;
        std::unique_ptr<FontRasterization::FontManager> m_font_manager;
    };

} // namespace
