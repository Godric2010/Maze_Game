#pragma once
#include <optional>
#include <unordered_map>

#include "TextController.hpp"

namespace Engine::Systems::UI {
    class UiCache {
    public:
        struct TextElement {
            uint64_t last_text_version;
            uint64_t last_font_version;
            std::pair<float, float> text_dimensions;
            std::optional<Text::FontHandle> font_handle;
            std::optional<Assets::TextureHandle> texture_handle;
            Assets::MaterialHandle material_handle;
            Assets::MeshHandle mesh_handle;
        };

        struct ColorElement {
            Assets::MeshHandle mesh_handle;
            Assets::MaterialHandle material_handle;
            glm::vec4 color;
        };

        UiCache();

        ~UiCache();

        void RegisterTextElement(uint64_t entity, TextElement text_element);

        void RegisterColorElement(uint64_t entity, ColorElement color_element);

        void DeregisterTextElement(uint64_t entity);

        void DeregisterColorElement(uint64_t entity);

        void SetTextElementValue(uint64_t entity, TextElement text_element);

        void SetColorElementValue(uint64_t entity, ColorElement color_element);

        TextElement &GetTextElement(uint64_t entity);

        ColorElement &GetColorElement(uint64_t entity);

    private:
        std::unordered_map<uint32_t, TextElement> m_text_cache;
        std::unordered_map<uint32_t, ColorElement> m_color_element_cache;
    };
} // namespace
