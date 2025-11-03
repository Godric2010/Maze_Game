#pragma once
#include "ISystem.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "GameWorld.hpp"
#include "../commands/PauseCommand.hpp"
#include "../commands/QuitCommand.hpp"
#include "ui/Button.hpp"
#include "ui/RectTransform.hpp"
#include <iostream>

namespace Gameplay::Systems {
    ECS_SYSTEM(PauseSystem, Update, [])

    class PauseSystem : public Engine::Ecs::ISystem {
    public:
        PauseSystem();

        ~PauseSystem() override;

        void Initialize() override;

        void Run(float delta_time) override;

    private:
        static bool IsInsideRect(const glm::vec2 &mouse_pos, const glm::vec2 &rect_pos, const glm::vec2 &rect_size);
    };
} // namespace
