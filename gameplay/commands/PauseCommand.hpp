//
// Created by Sebastian Borsch on 30.10.25.
//

#pragma once

namespace Gameplay::Commands {
    class PauseCommand {
    public:
        explicit PauseCommand(const bool is_paused) { m_is_paused = is_paused; };

        ~PauseCommand() = default;

        [[nodiscard]] bool IsPaused() const { return m_is_paused; };

    private:
        bool m_is_paused;
    };
}
