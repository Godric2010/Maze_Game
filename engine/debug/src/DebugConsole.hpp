#pragma once
#include "TextController.hpp"
#include "../../renderer/RenderController.hpp"
#include "../include/IDebugConsole.hpp"

namespace Engine::Debug {
    struct TextMeshElement {
        Renderer::MeshHandle mesh_handle;
        float width;
        float height;
    };

    struct TextElement {
        std::string label;
        std::string content;
        TextMeshElement label_mesh;
        TextMeshElement content_mesh;
    };

    class DebugConsole : public IDebugConsole {
    public:
        DebugConsole(Text::TextController* text_controller, Renderer::RenderController* render_controller,
                     const Environment::WindowContext& context, uint32_t column_width);

        ~DebugConsole() override;

        void PushValue(const std::string& label, size_t value) override;

        void PushToFrame() override;

    private:
        Text::TextController* m_text_controller;
        Renderer::RenderController* m_render_controller;
        float m_window_width;
        float m_window_height;

        float m_column_width;

        const int m_font_size = 24;
        Text::FontHandle m_font_handle;
        Renderer::TextureHandle m_texture_handle;

        uint8_t m_current_label_id = 0;
        std::unordered_map<std::string, uint8_t> m_label_id_map;
        std::unordered_map<uint8_t, TextElement> m_text_elements;

        void UpdateTextElements(uint8_t id, const std::string& content);

        [[nodiscard]] TextElement CreateTextElement(const std::string& label,
                                                    const std::string& content) const;

        [[nodiscard]] TextMeshElement CreateTextMeshElement(const std::string& text) const;

        Renderer::UiDrawAsset CreateUiDrawAsset(uint8_t col, uint8_t row, TextMeshElement text_mesh_element) const;
    };
} // namespace
