#include "UiSystem.hpp"

#include "GameWorld.hpp"
#include "../../text/include/TextController.hpp"
#include "ui/Text.hpp"


namespace Engine::Core::Systems {
    UiSystem::UiSystem() = default;

    void UiSystem::Initialize() {
    }

    void UiSystem::Run(float delta_time) {
        HandleTextLabels();

        const auto input = GameWorld()->GetInput();
        if (!input.IsMapActive("UIInputMap")) {
            return;
        }
        HandleButtons(input);
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

    void UiSystem::HandleButtons(const InputBuffer &input) const {
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

    void UiSystem::HandleTextLabels() const {
        auto text_labels = GameWorld()->GetComponentsOfType<Components::UI::Text>();
        for (const auto text: text_labels | std::views::keys) {
            if (text->IsDirty()) {
                // Do something with the text here
                text->m_text_parameters.font = ServiceLocator()->GetService<Text::TextController>()->LoadFont(
                    text->GetFontName(), text->m_text_parameters.font.font_size);
                text->m_is_dirty = false;
            }
        }
    }
} // namespace
