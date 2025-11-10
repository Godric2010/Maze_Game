//
// Created by Sebastian Borsch on 09.11.25.
//

#pragma once
#include <string>
#include <optional>
#include <utility>
#include "TextMeshHandle.hpp"
#include "TextParameters.hpp"

namespace Engine::Core::Systems {
    class UiSystem;
}

namespace Engine::Core::Components::UI {
    struct Text {
        explicit Text(std::string text, const std::string &font_name, const int font_size) : m_is_dirty(true) {
            m_text_parameters = Engine::Text::TextParameters{
                .font = Engine::Text::FontHandle{.font_name = font_name, .font_size = font_size},
                .text = std::move(text),
            };
            m_text_mesh = std::nullopt;
        }

        [[nodiscard]] std::string GetText() const { return m_text_parameters.text; }
        [[nodiscard]] std::string GetFontName() const { return m_text_parameters.font.font_name; }
        [[nodiscard]] bool IsDirty() const { return m_is_dirty; }

        [[nodiscard]] Engine::Text::TextMeshHandle GetTextMeshHandle() const {
            if (!m_text_mesh.has_value()) {
                throw std::invalid_argument("Text is not a valid text");
            }
            return m_text_mesh.value();
        }

        void SetText(const std::string &text) {
            m_text_parameters.text = text;
            m_is_dirty = true;
        }

    private:
        friend class Systems::UiSystem;

        Engine::Text::TextParameters m_text_parameters;
        std::optional<Engine::Text::TextMeshHandle> m_text_mesh;
        bool m_is_dirty;
    };
}
