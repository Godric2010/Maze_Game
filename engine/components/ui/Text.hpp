//
// Created by Sebastian Borsch on 09.11.25.
//

#pragma once
#include <string>

namespace Engine::Components::UI {
    struct Text {
        Text() {
            m_text_content = "";
            m_font_name = "";
            m_text_size_in_px = 0;
            m_text_version = 0;
            m_font_version = 0;
        }

        [[nodiscard]] std::string GetText() const { return m_text_content; }
        [[nodiscard]] std::string GetFontName() const { return m_font_name; }
        [[nodiscard]] int GetFontSize() const { return m_text_size_in_px; }
        [[nodiscard]] uint64_t GetFontVersion() const { return m_font_version; }
        [[nodiscard]] uint64_t GetTextVersion() const { return m_text_version; }

        Text &SetText(const std::string& text) {
            m_text_content = text;
            m_text_version++;
            return *this;
        }

        Text &SetFontName(const std::string& font_name) {
            m_font_name = font_name;
            m_font_version++;
            return *this;
        }

        Text &SetFontSize(const int font_size) {
            m_text_size_in_px = font_size;
            m_font_version++;
            return *this;
        }

    private:
        std::string m_text_content;
        std::string m_font_name;
        int m_text_size_in_px;
        uint64_t m_text_version;
        uint64_t m_font_version;
    };
}
