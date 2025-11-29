//
// Created by Sebastian Borsch on 26.11.25.
//

#pragma once
#include "EnvironmentBuilder.hpp"
#include "Input/IInput.hpp"
#include "Input/InputTypes.hpp"

namespace Engine::Input {
    class InputManager;

    class IInputManager : public IInput {
    public:
        virtual void UpdateInput() = 0;

        virtual Environment::AppEventsSnapshot* GetAppEventSnapshot() = 0;

        virtual void AddInputMapping(const InputMap& input_map) = 0;

    };
}
