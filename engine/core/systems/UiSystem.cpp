#include "UiSystem.hpp"

#include "GameWorld.hpp"


namespace Engine::Core::Systems {
    UiSystem::UiSystem() = default;

    void UiSystem::Initialize() {
    }

    void UiSystem::Run(float delta_time) {
        const auto input = GameWorld()->GetInput();
        if (!input.IsMapActive("UIInputMap")) {
            return;
        }

        auto buttons_with_entities = GameWorld()->GetComponentsOfType<Components::UI::Button>();
        for (auto [button, entity]: buttons_with_entities) {
            const auto rect = GameWorld()->GetComponent<Components::UI::RectTransform>(entity);
            if (!button->enabled) {
                button->m_current_color = button->disabled_color;
                continue;
            }

            button->m_current_color = button->default_color;
            if (IsMouseOverElement(input.mouse_position, rect)) {
                button->m_current_color = button->highlight_color;

                if (input.HasAction("UiButtonDown")) {
                    button->m_current_color = button->click_color;
                } else if (input.HasAction("UiButtonUp")) {
                    const auto command = Commands::UI::ButtonClickedCommand(button->button_id);
                    GameWorld()->PushCommand(command);
                }
            }
        }
    }

    bool UiSystem::IsMouseOverElement(const glm::vec2 mouse_pos, const Components::UI::RectTransform *rect) {
        if (mouse_pos.x > rect->Position().x - rect->Size().x * rect->Pivot().x &&
            mouse_pos.y > rect->Position().y - rect->Size().y * rect->Pivot().y &&
            mouse_pos.x < rect->Position().x + rect->Size().x * rect->Pivot().x &&
            mouse_pos.y < rect->Position().y + rect->Size().y * rect->Pivot().y) {
            return true;
        }
        return false;
    }
} // namespace
