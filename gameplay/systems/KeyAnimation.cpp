#include "KeyAnimation.hpp"

#include "SystemWorld.hpp"
#include "Transform.hpp"
#include "../components/KeyItem.hpp"

namespace gameplay::systems {
    void KeyAnimation::Initialize() {
    }

    void KeyAnimation::Run(float delta_time) {
        auto entities = GameWorld()->GetComponentsOfType<components::KeyItem>();
        for (const auto entity_id: entities | std::views::values) {
            const auto transform = GameWorld()->GetComponent<yarep::components::Transform>(entity_id);

            auto rotation = transform->GetRotation();
            rotation.y += 10 * delta_time * m_rotation_speed;
            transform->SetRotation(rotation);

            auto position = transform->GetPosition();
            if (position.y > m_max_height && m_goes_up) {
                m_goes_up = false;
                m_hover_direction *= -1.0f;
            }
            if (position.y < m_min_height && !m_goes_up) {
                m_goes_up = true;
                m_hover_direction *= -1.0f;
            }

            position += m_hover_direction * delta_time * m_hover_speed;
            transform->SetPosition(position);
        }
    }
} // namespace
