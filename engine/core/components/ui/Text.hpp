//
// Created by Sebastian Borsch on 09.11.25.
//

#pragma once
#include <string>
#include <optional>
#include "Types.hpp"
#include "../renderer/Datatypes.hpp"

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
        [[nodiscard]] std::optional<std::pair<float, float> > GetTextDimensions() const { return m_text_dimensions; }
        [[nodiscard]] bool IsDirty() const { return m_is_dirty; }


        [[nodiscard]] Engine::Text::FontHandle GetFontHandle() const {
            if (!m_font_handle.has_value()) {
                throw std::runtime_error("Font handle has not been set!");
            }
            return m_font_handle.value();
        }

        [[nodiscard]] std::optional<Renderer::MeshHandle> GetTextMesh() const {
            return m_text_mesh;
        }

        [[nodiscard]] std::optional<Renderer::TextureHandle> GetTextureHandle() const {
            return m_texture_handle;
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


        std::optional<std::pair<float, float> > m_text_dimensions;
        std::optional<Engine::Text::FontHandle> m_font_handle;
        std::optional<Renderer::MeshHandle> m_text_mesh;
        std::optional<Renderer::TextureHandle> m_texture_handle;
        bool m_is_dirty;
    };
}
