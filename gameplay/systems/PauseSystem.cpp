#include "PauseSystem.hpp"

#include <iostream>
#include "../commands/PauseCommand.hpp"


namespace gameplay::systems {
    PauseSystem::PauseSystem() = default;

    PauseSystem::~PauseSystem() = default;

    void PauseSystem::Initialize() {
    }

    void PauseSystem::Run(float delta_time) {
        const auto input = Input()->GetInput();
        if (!input.IsMapActive("PauseInputMap"))
            return;

        if (input.HasAction("resume")) {
            std::cout << "Resume game!" << std::endl;
            const auto pause_command = commands::PauseCommand(false);
            SendCommand(pause_command);
            return;
        }
    }
} // namespace
