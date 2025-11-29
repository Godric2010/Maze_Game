//
// Created by Sebastian Borsch on 26.11.25.
//

#pragma once
#include <string>
#include <vector>

namespace Engine::Input {
    enum class Key {
        Unknown,
        W,
        A,
        S,
        D,
        Enter,
        Esc,
        Space,
        ShiftLeft,
    };

    enum class MouseButton {
        Unknown,
        Left,
        Right,
        Middle,
    };

    enum class PressState {
        Down,
        Up,
        Pressed,
    };

    struct KeyBinding {
        std::string name;
        Key key;
        PressState press_state;
    };

    struct MouseKeyBinding {
        std::string name;
        MouseButton button;
        PressState press_state;
    };

    struct InputMap {
        std::string name;
        std::vector<KeyBinding> key_bindings;
        std::vector<MouseKeyBinding> mouse_bindings;
    };

    struct InputAction {
        std::string name;
    };
}
