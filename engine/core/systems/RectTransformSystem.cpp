#include "RectTransformSystem.hpp"

#include <glm/ext/matrix_transform.hpp>

#include "GameWorld.hpp"

namespace Engine::Core::Systems {
    void RectTransformSystem::Initialize() {
    }

    void RectTransformSystem::Run(float delta_time) {
        auto rect_transform_components = GameWorld()->GetComponentsOfType<Components::UI::RectTransform>();
        for (const auto rect_transform_component: rect_transform_components | std::views::keys) {
            if (!rect_transform_component->m_is_dirty) {
                continue;
            }

            const auto layout_result = CreateUiLayoutResult(*rect_transform_component);
            rect_transform_component->m_layout_result = layout_result;
            rect_transform_component->m_is_dirty = false;
        }
    }

    glm::vec2 RectTransformSystem::GetAnchorValue(const Components::UI::Anchor &anchor) {
        switch (anchor) {
            case Components::UI::Anchor::TopLeft:
                return {0.0f, 0.0f};
            case Components::UI::Anchor::TopCenter:
                return {0.5f, 0.0f};
            case Components::UI::Anchor::TopRight:
                return {1.0f, 0.0f};
            case Components::UI::Anchor::CenterLeft:
                return {0.0f, 0.5f};
            case Components::UI::Anchor::Center:
                return {0.5f, 0.5f};
            case Components::UI::Anchor::CenterRight:
                return {1.0f, 0.5f};
            case Components::UI::Anchor::BottomLeft:
                return {0.0f, 1.0f};
            case Components::UI::Anchor::BottomCenter:
                return {0.5f, 1.0f};
            case Components::UI::Anchor::BottomRight:
                return {1.0f, 1.0f};
            default:
                throw std::runtime_error("Unknown Anchor type");
        };
    }

    Components::UI::UiLayoutResult RectTransformSystem::CreateUiLayoutResult(
        const Components::UI::RectTransform &rect_transform) {
        Components::UI::UiLayoutResult result{};

        glm::vec2 anchor_point;
        if (rect_transform.GetParent().has_value()) {
            // TODO: use the parent here
            anchor_point = glm::vec2(0, 0);
        } else {
            anchor_point = m_world_origin + m_world_scale * GetAnchorValue(rect_transform.GetAnchor());
        }

        const glm::vec2 global_position = anchor_point + rect_transform.GetLocalPosition() - rect_transform.GetLocalSize() *
                                          rect_transform.GetPivot();

        result.global_position = global_position;
        result.global_size = rect_transform.GetLocalSize(); // TODO: Handle special cases like text size here!

        auto matrix = glm::mat4(1.0f);
        matrix = scale(matrix, glm::vec3(result.global_size.x, result.global_size.y, 0.0f));
        matrix = translate(matrix, glm::vec3(result.global_position.x, result.global_position.y, 0.0f));
        result.global_matrix = matrix;

        return result;
    }
} // namespace
