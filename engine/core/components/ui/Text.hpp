//
// Created by Sebastian Borsch on 09.11.25.
//

#pragma once
#include <string>
#include <optional>
#include <utility>
#include "Types.hpp"

namespace Engine::Core::Systems {
    class UiSystem;
}

namespace Engine::Core::Components::UI {
    struct Text {
        explicit Text(const std::string &text, const std::string &font_name, const int font_size) : m_is_dirty(true) {
            m_text_content = text;
            m_font_name = font_name;
            m_text_size_in_px = font_size;
            m_text_mesh = std::nullopt;
        }

        [[nodiscard]] std::string GetText() const { return m_text_content; }
        [[nodiscard]] std::string GetFontName() const { return m_font_name; }
        [[nodiscard]] int GetFontSize() const { return m_text_size_in_px; }
        [[nodiscard]] bool IsDirty() const { return m_is_dirty; }


        [[nodiscard]] Engine::Text::FontHandle GetFontHandle() const {
            if (!m_font_handle.has_value()) {
                throw std::runtime_error("Font handle has not been set!");
            }
            return m_font_handle.value();
        }

        [[nodiscard]] Engine::Text::TextMesh GetTextMesh() const {
            if (!m_text_mesh.has_value()) {
                throw std::invalid_argument("Text mesh has not been initialized");
            }
            return m_text_mesh.value();
        }

        void SetText(const std::string &text) {
            m_text_content = text;
            m_is_dirty = true;
        }

    private:
        friend class Systems::UiSystem;

        std::string m_text_content;
        std::string m_font_name;
        int m_text_size_in_px;

        std::optional<Engine::Text::FontHandle> m_font_handle;
        std::optional<Engine::Text::TextMesh> m_text_mesh;
        bool m_is_dirty;
    };
}
