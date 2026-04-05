#include "InputManagerBuilder.hpp"
#include "InputManager.hpp"

namespace Engine::Input {
    std::unique_ptr<IInputManager> InputManagerBuilder::CreateInputManager(Environment::IWindow* window, const std::vector<InputMap>& input_maps) {
        auto input = std::make_unique<InputManager>(Environment::EnvironmentBuilder::CreateInput(*window), input_maps);
        return input;
    }
}
