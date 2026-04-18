#include "KeyAnimation.hpp"

#include "SystemWorld.hpp"
#include "Transform.hpp"
#include "../components/KeyItem.hpp"

namespace Gameplay::Systems {
    void KeyAnimation::Initialize() {
    }

    void KeyAnimation::Run(float delta_time) {
        auto entities = GameWorld()->GetComponentsOfType<Components::KeyItem>();
        for (const auto entityId: entities | std::views::values) {
            const auto transform = GameWorld()->GetComponent<Engine::Components::Transform>(entityId);

            auto rotation = transform->GetRotation();
            rotation.y += 10 * delta_time * m_rotation_speed;
            transform->SetRotation(rotation);

            auto position = transform->GetPosition();
            if (position.y > m_max_height && goes_up) {
                goes_up = false;
                hover_direction *= -1.0f;
            }
            if (position.y < m_min_height && !goes_up) {
                goes_up = true;
                hover_direction *= -1.0f;
            }

            position += hover_direction * delta_time * m_hover_speed;
            transform->SetPosition(position);
        }
    }
} // namespace
