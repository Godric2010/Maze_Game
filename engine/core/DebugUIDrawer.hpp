#pragma once
#include <string>
#include <unordered_map>

#include "TextController.hpp"
#include "../renderer/Datatypes.hpp"
#include "../renderer/RenderController.hpp"

namespace Engine::Core {
    struct DebugElement {
        uint8_t id;
        std::string content;
        Renderer::MeshHandle mesh_handle;
        float text_width;
        float text_height;
    };

    class DebugUiDrawer {
    public:
        DebugUiDrawer(Text::TextController* text_controller, Renderer::RenderController* render_controller,
                      const Environment::WindowContext& context);

        ~DebugUiDrawer();

        void SetDrawData(std::unordered_map<uint8_t, std::string>& data);

        std::vector<Renderer::UiDrawAsset> DrawDebugUi();

    private:
        Text::TextController* m_text_controller;
        Renderer::RenderController* m_render_controller;
        uint32_t m_window_width;
        uint32_t m_window_height;

        const int m_font_size = 24;
        Text::FontHandle m_font_handle;
        Renderer::TextureHandle m_texture_handle;

        std::unordered_map<uint8_t, DebugElement> m_text_elements;

        void UpdateTextElements(uint8_t id, const std::string& content);

        [[nodiscard]] DebugElement CreateTextElements(uint8_t id, const std::string& content) const;
    };
} // namespace
