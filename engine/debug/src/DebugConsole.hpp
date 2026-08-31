#pragma once
#include <IRenderController.hpp>
#include <Window.hpp>
#include "TextController.hpp"
#include "../include/IDebugConsole.hpp"

namespace yarep::debug
{
    struct TextMeshElement
    {
        assets::MeshHandle mesh_handle;
        assets::MaterialHandle material_handle;
        float width;
        float height;
    };

    struct TextElement
    {
        std::string label;
        std::string content;
        TextMeshElement label_mesh;
        TextMeshElement content_mesh;
    };

    class DebugConsole : public IDebugConsole
    {
    public:
        DebugConsole(text::TextController* text_controller, renderer::IRenderController* render_controller,
                     asset_handling::AssetHandler* asset_handler,
                     const environment::WindowContext& context, uint32_t column_width);

        ~DebugConsole() override;

        void PushValue(const std::string& label, size_t value) override;

        void PushToFrame() override;

    private:
        text::TextController* m_text_controller;
        renderer::IRenderController* m_render_controller;
        asset_handling::AssetHandler* m_asset_handler;
        float m_window_width;
        float m_window_height;

        float m_column_width;

        const int m_font_size = 24;
        text::FontHandle m_font_handle;
        assets::TextureHandle m_texture_handle;

        uint8_t m_current_label_id = 0;
        std::unordered_map<std::string, uint8_t> m_label_id_map;
        std::unordered_map<uint8_t, TextElement> m_text_elements;

        void UpdateTextElements(uint8_t id, const std::string& content);

        [[nodiscard]] TextElement CreateTextElement(const std::string& label,
                                                    const std::string& content) const;

        [[nodiscard]] TextMeshElement CreateTextMeshElement(const std::string& text) const;

        renderer::DrawAsset CreateUiDrawAsset(uint8_t col, uint8_t row, const TextMeshElement& text_mesh_element, uint8_t queue_index) const;
    };
} // namespace
