#pragma once
#include <glm/vec4.hpp>

#include "TextController.hpp"

namespace Engine::Systems::UI {
    class UiCache {
    public:
        struct TextElement {
        };

        struct ButtonElement {
            glm::vec4 color;
        };

        UiCache();

        ~UiCache();

        void RegisterTextEntity(uint64_t entity);

        void RegisterButtonEntity(uint64_t entity);

        void DeregisterTextEntity(uint64_t entity);

        void DeregisterButtonEntity(uint64_t entity);

        void SetTextElementValue(uint64_t entity, TextElement text_element);

        void SetButtonElementValue(uint64_t entity, ButtonElement button_element);

        TextElement &GetTextElement(uint64_t entity);

        ButtonElement &GetButtonElement(uint64_t entity);

    private:
        std::unordered_map<uint32_t, TextElement> m_text_cache;
        std::unordered_map<uint32_t, ButtonElement> m_button_cache;
    };
} // namespace
