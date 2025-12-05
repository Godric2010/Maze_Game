#include "RectTransformSystem.hpp"

#include <glm/ext/matrix_transform.hpp>

namespace Engine::Core::Systems {
    void RectTransformSystem::Initialize() {
    }

    void RectTransformSystem::Run(float delta_time) {
        m_calculated_layouts.clear();
        m_dirty_rect_transforms.clear();

        const auto rect_transform_components = EcsWorld()->GetComponentsOfType<Components::UI::RectTransform>();
        for (const auto [rect_transform, entity]: rect_transform_components) {
            if (!rect_transform->m_is_dirty) {
                continue;
            }
            m_dirty_rect_transforms.emplace(entity, rect_transform);
        }

        for (const auto [entity, dirty_rect]: m_dirty_rect_transforms) {
            if (m_calculated_layouts.contains(entity)) {
                continue;
            }
            const auto layout_data = CreateLayoutData(dirty_rect);
            const auto ui_layout = CreateUiLayoutResult(layout_data);
            m_calculated_layouts.emplace(entity, ui_layout);
        }

        for (const auto [entity, layout]: m_calculated_layouts) {
            m_dirty_rect_transforms[entity]->m_layout_result = layout;
            m_dirty_rect_transforms[entity]->m_is_dirty = false;
        }
    }

    glm::vec2 RectTransformSystem::GetAnchorValue(const Components::UI::Anchor& anchor) {
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

    LayoutData RectTransformSystem::CreateLayoutData(const Components::UI::RectTransform* rect_transform) {
        LayoutData result{};
        result.local_position = rect_transform->GetLocalPosition();
        result.local_size = rect_transform->GetLocalSize();
        result.pivot = rect_transform->GetPivot();

        const auto anchor_value = GetAnchorValue(rect_transform->GetAnchor());

        if (rect_transform->GetParent().has_value()) {
            const auto parent_layout = GetParentLayoutResult(rect_transform->GetParent().value());
            result.parent_layer = parent_layout.layer;
            result.anchor_point = parent_layout.global_position + parent_layout.global_size * anchor_value;
        } else {
            result.anchor_point = m_world_origin + m_world_scale * anchor_value;
            result.parent_layer = 0;
        }
        return result;
    }

    Components::UI::UiLayoutResult RectTransformSystem::CreateUiLayoutResult(const LayoutData& rect_layout) {
        Components::UI::UiLayoutResult result{};

        const glm::vec2 global_position = rect_layout.anchor_point + rect_layout.local_position - rect_layout.local_size
                                          * rect_layout.pivot;

        result.global_position = global_position;
        result.global_size = rect_layout.local_size;
        result.layer = rect_layout.parent_layer + 1;

        auto matrix = glm::mat4(1.0f);
        matrix = translate(matrix, glm::vec3(result.global_position.x, result.global_position.y, 0.0f));
        matrix = scale(matrix, glm::vec3(result.global_size.x, result.global_size.y, 1.0f));
        result.global_matrix = matrix;

        return result;
    }

    Components::UI::UiLayoutResult RectTransformSystem::GetParentLayoutResult(const Ecs::EntityId& entity) {
        if (m_calculated_layouts.contains(entity)) {
            return m_calculated_layouts[entity];
        }

        const auto parent_rect_transform = EcsWorld()->GetComponent<Components::UI::RectTransform>(entity);
        const auto layout_data = CreateLayoutData(parent_rect_transform);
        auto layout = CreateUiLayoutResult(layout_data);
        if (m_dirty_rect_transforms.contains(entity)) {
            m_calculated_layouts.emplace(entity, layout);
        }
        return layout;
    }
} // namespace
