//
// Created by Sebastian Borsch on 03.11.25.
//

#pragma once
#include <cstdint>

namespace Engine::Core::Commands::UI {
    class ButtonClickedCommand {
    public:
        explicit ButtonClickedCommand(const uint32_t button_id) {
            m_button_id = button_id;
        };

        ~ButtonClickedCommand() = default;

        [[nodiscard]] uint32_t GetButtonId() const { return m_button_id; };

    private:
        uint32_t m_button_id;
    };
}
