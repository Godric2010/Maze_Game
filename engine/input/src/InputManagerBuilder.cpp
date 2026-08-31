#include "InputManagerBuilder.hpp"
#include "InputManager.hpp"

namespace yarep::input {
    std::unique_ptr<IInputManager> InputManagerBuilder::CreateInputManager(environment::IWindow* window, const std::vector<InputMap>& input_maps) {
        auto input = std::make_unique<InputManager>(environment::EnvironmentBuilder::CreateInput(*window), input_maps);
        return input;
    }
}
