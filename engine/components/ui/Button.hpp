//
// Created by Sebastian Borsch on 02.11.25.
//

#pragma once
#include <glm/vec4.hpp>


namespace Engine::Components::UI {
    enum class ButtonState {
        Default,
        Highlighted,
        Pressed,
        Disabled,
    };

    struct Button {
    public:
        uint32_t button_id;
        glm::vec4 default_color;
        glm::vec4 highlight_color;
        glm::vec4 click_color;
        glm::vec4 disabled_color;
        bool enabled;

    private:
    };
}
