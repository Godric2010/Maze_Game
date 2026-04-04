//
// Created by Sebastian Borsch on 26.11.25.
//

#pragma once
#include <array>
#include <string>
#include <vector>

namespace Engine::Input
{
    #define KEY_LIST \
        X(Unknown) \
        X(W) \
        X(A) \
        X(S) \
        X(D) \
        X(Enter) \
        X(Esc) \
        X(Space) \
        X(ShiftLeft)

    enum class Key
    {
        #define X(name) name,
        KEY_LIST
        #undef X
    };

    constexpr std::array<std::pair<std::string_view, Key>, 9> KeyMap =
    {
        {
            #define X(name) {#name, Key::name},
            KEY_LIST
            #undef X
        }
    };

    #define MouseButtonList \
        X(Unknown)  \
        X(Left) \
        X(Right) \
        X(Middle)

    enum class MouseButton
    {
        #define X(name) name,
        MouseButtonList
        #undef X
    };

    constexpr std::array<std::pair<std::string_view, MouseButton>, 4> MouseButtonMap = {
        {
            #define X(name) {#name, MouseButton::name},
            MouseButtonList
            #undef X
        }
    };

    #define PressStateList \
        X(Down) \
        X(Up) \
        X(Pressed)


    enum class PressState
    {
        #define X(name) name,
        PressStateList
        #undef X
    };

    constexpr std::array<std::pair<std::string_view, PressState>, 3> PressStateMap = {
        {
            #define X(name) {#name, PressState::name},
            PressStateList
            #undef X
        }
    };

    struct KeyBinding
    {
        std::string name;
        Key key;
        PressState press_state;
    };

    struct MouseKeyBinding
    {
        std::string name;
        MouseButton button;
        PressState press_state;
    };

    struct InputMap
    {
        std::string name;
        std::vector<KeyBinding> key_bindings;
        std::vector<MouseKeyBinding> mouse_bindings;
    };

    struct InputAction
    {
        std::string name;
    };
}
