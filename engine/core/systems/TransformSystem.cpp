#include "TransformSystem.hpp"

#include <iostream>
#include <ostream>
#include <glm/ext/matrix_transform.hpp>

#include "GameWorld.hpp"
#include "components/Transform.hpp"

namespace Engine::Core::Systems {
    TransformSystem::TransformSystem() = default;

    TransformSystem::~TransformSystem() = default;

    void TransformSystem::Initialize() {
    }

    void TransformSystem::Run(float delta_time) {
        auto transform_components = GameWorld()->GetComponentsOfType<Components::Transform>();
        for (const auto transform: transform_components | std::views::keys) {
            if (!transform->IsDirty()) {
                continue;
            }
            auto matrix = CalculateModelMatrix(transform->GetPosition(), transform->GetRotation(),
                                               transform->GetScale());
            transform->SetMatrix(matrix);
        }

        auto rect_transform_components = GameWorld()->GetComponentsOfType<Components::UI::RectTransform>();
        for (const auto rect_transform_component: rect_transform_components | std::views::keys) {
            const auto matrix = CalculateUiModelMatrix(rect_transform_component);
            rect_transform_component->m_matrix = matrix;
        }
    }

    glm::mat4 TransformSystem::CalculateModelMatrix(const glm::vec3 position, const glm::vec3 rotation,
                                                    const glm::vec3 scale) {
        const auto matrix = glm::translate(glm::mat4(1.0), position) *
                            rotate(glm::mat4(1.0), glm::radians(rotation.z), glm::vec3(0, 0, 1)) *
                            rotate(glm::mat4(1.0), glm::radians(rotation.y), glm::vec3(0, 1, 0)) *
                            rotate(glm::mat4(1.0), glm::radians(rotation.x), glm::vec3(1, 0, 0)) *
                            glm::scale(glm::mat4(1.0), scale);
        return matrix;
    }

    glm::mat4 TransformSystem::CalculateUiModelMatrix(const Components::UI::RectTransform *rect_transform) {
        glm::mat4 scale_mat(1.0f);
        scale_mat[0][0] = rect_transform->m_size.x;
        scale_mat[1][1] = rect_transform->m_size.y;

        const auto pivot_offset = glm::vec2(rect_transform->m_pivot.x * rect_transform->m_size.x,
                                            rect_transform->m_pivot.y * rect_transform->m_size.y);

        glm::mat4 pivot_mat(1.0f);
        pivot_mat[3][0] = -pivot_offset.x;
        pivot_mat[3][1] = -pivot_offset.y;

        const float rot = rect_transform->m_rotation;
        const float cos_rot = glm::cos(rot);
        const float sin_rot = glm::sin(rot);

        auto rotation_mat = glm::mat4(1.0f);
        rotation_mat[0][0] = cos_rot;
        rotation_mat[0][1] = sin_rot;
        rotation_mat[1][0] = -sin_rot;
        rotation_mat[1][1] = cos_rot;

        constexpr auto screen_mat = glm::mat4(1.0f);
        scale_mat[3][0] = rect_transform->m_position.x;
        scale_mat[3][1] = rect_transform->m_position.y;
        scale_mat[3][2] = rect_transform->m_layer_z;

        return screen_mat * rotation_mat * pivot_mat * scale_mat;
    }
} // namespace
