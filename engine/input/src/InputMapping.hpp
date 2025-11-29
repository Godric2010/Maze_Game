//
// Created by Sebastian Borsch on 26.11.25.
//

#pragma once
#include <unordered_map>

#include "EnvInput.hpp"
#include "Input/InputTypes.hpp"

namespace Engine::Input {
    struct InputMapping {
        std::unordered_map<Key, Environment::Key> keys{
            {Key::Unknown, Environment::Key::Unknown},
            {Key::A, Environment::Key::A},
            {Key::D, Environment::Key::D},
            {Key::S, Environment::Key::S},
            {Key::W, Environment::Key::W},
            {Key::Enter, Environment::Key::Enter},
            {Key::Esc, Environment::Key::Esc},
            {Key::ShiftLeft, Environment::Key::ShiftLeft},
            {Key::Space, Environment::Key::Space}
        };

        std::unordered_map<MouseButton, Environment::MouseButton> mouse{
            {MouseButton::Left, Environment::MouseButton::Left},
            {MouseButton::Right, Environment::MouseButton::Right},
            {MouseButton::Middle, Environment::MouseButton::Middle},
            {MouseButton::Unknown, Environment::MouseButton::Unknown}
        };

        std::unordered_map<PressState, Environment::PressState> press_state{
            {PressState::Down, Environment::PressState::Down},
            {PressState::Pressed, Environment::PressState::Pressed},
            {PressState::Up, Environment::PressState::Up},
        };
    };
}
