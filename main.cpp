#include <iostream>

#include "engine/core/EngineController.hpp"

int main() {
    const auto engineController = std::make_unique<Engine::Core::EngineController>();
    engineController->Initialize();
    engineController->Update();
    engineController->Shutdown();


    return 0;
}
