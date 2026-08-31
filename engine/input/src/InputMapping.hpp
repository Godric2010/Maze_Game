//
// Created by Sebastian Borsch on 26.11.25.
//

#pragma once
#include <unordered_map>

#include "EnvInput.hpp"
#include "Input/InputTypes.hpp"

namespace yarep::input {
    struct InputMapping {
        std::unordered_map<Key, environment::Key> keys{
            {Key::Unknown, environment::Key::Unknown},
            {Key::A, environment::Key::A},
            {Key::D, environment::Key::D},
            {Key::S, environment::Key::S},
            {Key::W, environment::Key::W},
            {Key::Enter, environment::Key::Enter},
            {Key::Esc, environment::Key::Esc},
            {Key::ShiftLeft, environment::Key::ShiftLeft},
            {Key::Space, environment::Key::Space}
        };

        std::unordered_map<MouseButton, environment::MouseButton> mouse{
            {MouseButton::Left, environment::MouseButton::Left},
            {MouseButton::Right, environment::MouseButton::Right},
            {MouseButton::Middle, environment::MouseButton::Middle},
            {MouseButton::Unknown, environment::MouseButton::Unknown}
        };

        std::unordered_map<PressState, environment::PressState> press_state{
            {PressState::Down, environment::PressState::Down},
            {PressState::Pressed, environment::PressState::Pressed},
            {PressState::Up, environment::PressState::Up},
        };
    };
}
