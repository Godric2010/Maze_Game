//
// Created by Sebastian Borsch on 02.11.25.
//

#pragma once
#include <glm/vec4.hpp>

namespace Engine::Core::Systems {
    class UiSystem;
}

namespace Engine::Core::Components::UI {
    enum class ButtonState {
        Default,
        Highlighted,
        Pressed,
        Disabled,
    };

    struct Button {
    private:
        friend class Systems::UiSystem;

    public:
        uint32_t button_id;
        glm::vec4 default_color;
        glm::vec4 highlight_color;
        glm::vec4 click_color;
        glm::vec4 disabled_color;
        bool enabled;

        [[nodiscard]] glm::vec4 GetColor() const { return m_current_color; }

    private:
        ButtonState m_state{};
        glm::vec4 m_current_color{};
    };
}
