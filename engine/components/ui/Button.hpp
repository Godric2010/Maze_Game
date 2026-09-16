//
// Created by Sebastian Borsch on 02.11.25.
//

#pragma once
#include <Math.hpp>


namespace yarep::components::ui {
    enum class ButtonState {
        Default,
        Highlighted,
        Pressed,
        Disabled,
    };

    struct Button {
    public:
        uint32_t button_id;
        math::Vec4 default_color;
        math::Vec4 highlight_color;
        math::Vec4 click_color;
        math::Vec4 disabled_color;
        bool enabled;

    private:
    };
}
