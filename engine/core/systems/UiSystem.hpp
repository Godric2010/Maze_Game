#pragma once
#include "ISystem.hpp"
#include "ui/Button.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include "ui/RectTransform.hpp"
#include "../commands/ui/ButtonClickedCommand.hpp"

namespace Engine::Core::Systems {
    ECS_SYSTEM(UiSystem, Ui, [ENGINE])
    class UiSystem : public Ecs::IEngineSystem {
    public:
        UiSystem();

        ~UiSystem() override = default;

        void Initialize() override;

        void Run(float delta_time) override;

    private:
        static bool IsMouseOverElement(glm::vec2 mouse_pos, const Components::UI::RectTransform* rect);
    };
} // namespace
