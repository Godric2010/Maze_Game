#pragma once
#include <glm/vec3.hpp>

#include "Ecs/ISystem.hpp"

namespace Gameplay::Systems {
    ECS_SYSTEM(KeyAnimation, Update, TAGS(), DEPENDENCIES())

    class KeyAnimation : public Engine::Ecs::ISystem {
    public:
        KeyAnimation() = default;

        ~KeyAnimation() override = default;

        void Initialize() override;

        void Run(float delta_time) override;

    private:
        float m_rotation_speed = 5.0f;
        float m_hover_speed = 0.08f;

        float m_min_height = 0.4f;
        float m_max_height = 0.6f;

        glm::vec3 hover_direction = glm::vec3(0.0f, 1.0f, 0.0f);
        bool goes_up = true;
    };
} // namespace
