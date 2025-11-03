#include "PauseSystem.hpp"


namespace Gameplay::Systems {
    PauseSystem::PauseSystem() = default;

    PauseSystem::~PauseSystem() = default;

    void PauseSystem::Initialize() {
    }

    void PauseSystem::Run(float delta_time) {
        const auto input = GameWorld()->GetInput();
        if (input.active_map_name != "PauseInputMap")
            return;

        if (input.HasAction("resume")) {
            std::cout << "Resume game!" << std::endl;
            const auto pause_command = Commands::PauseCommand(false);
            GameWorld()->PushCommand(pause_command);
            return;
        }

        auto buttons = GameWorld()->GetComponentsOfType<Engine::Core::Components::UI::Button>();
        for (const auto [button, entity]: buttons) {
            button->active_color = button->default_color;
            const auto rect = GameWorld()->GetComponent<Engine::Core::Components::UI::RectTransform>(entity);

            const bool is_inside_rect = IsInsideRect(input.mouse_position, rect->Position(), rect->Size());
            const bool is_clicked = input.HasAction("confirm");
            if (is_inside_rect) {
                if (is_clicked) {
                    button->active_color = button->click_color;
                    if (button->name == "Resume") {
                        const auto pause_command = Commands::PauseCommand(false);
                        GameWorld()->PushCommand(pause_command);
                    } else if (button->name == "Quit") {
                        const auto quit_command = Commands::QuitCommand();
                        GameWorld()->PushCommand(quit_command);
                    }
                } else {
                    button->active_color = button->highlight_color;
                }
            }
        }
    }

    bool PauseSystem::IsInsideRect(const glm::vec2 &mouse_pos, const glm::vec2 &rect_pos, const glm::vec2 &rect_size) {
        if (mouse_pos.x > rect_pos.x &&
            mouse_pos.y > rect_pos.y &&
            mouse_pos.x < rect_pos.x + rect_size.x &&
            mouse_pos.y < rect_pos.y + rect_size.y) {
            return true;
        }
        return false;
    }
} // namespace
