#include "PauseSystem.hpp"


namespace Gameplay::Systems {
    PauseSystem::PauseSystem() = default;

    PauseSystem::~PauseSystem() = default;

    void PauseSystem::Initialize() {
    }

    void PauseSystem::Run(float delta_time) {
        const auto input = GameWorld()->GetInput();
        if (!input.IsMapActive("PauseInputMap"))
            return;

        if (input.HasAction("resume")) {
            std::cout << "Resume game!" << std::endl;
            const auto pause_command = Commands::PauseCommand(false);
            GameWorld()->PushCommand(pause_command);
            return;
        }
    }
} // namespace
