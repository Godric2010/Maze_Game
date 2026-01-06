#include "InputManagerBuilder.hpp"
#include "InputManager.hpp"

namespace Engine::Input {
    std::unique_ptr<IInputManager> InputManagerBuilder::CreateInputManager(Environment::IWindow* window) {
        auto input = std::make_unique<InputManager>(CreateInput(*window));
        return input;
    }
}
