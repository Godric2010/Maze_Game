//
// Created by Sebastian Borsch on 28.10.25.
//

#pragma once
#include <glm/vec2.hpp>
#include <glm/mat4x4.hpp>
#include <optional>

namespace Engine::Core::Systems {
    class RectTransformSystem;
}

namespace Engine::Core::Components::UI {
    enum class Anchor {
        TopLeft,
        TopCenter,
        TopRight,
        CenterLeft,
        Center,
        CenterRight,
        BottomLeft,
        BottomCenter,
        BottomRight,
    };

    struct UiLayoutResult {
        glm::vec2 global_position = glm::vec2(0.0f);
        glm::vec2 global_size = glm::vec2(0.0f);
        glm::mat4 global_matrix = glm::mat4(1.0f);
        uint8_t layer = 0;
    };

    struct RectTransform {
    private:
        friend Systems::RectTransformSystem;

    public:
        RectTransform() = default;

        [[nodiscard]] glm::vec2 GetLocalPosition() const { return m_local_position; }
        [[nodiscard]] glm::vec2 GetLocalSize() const { return m_size; }
        [[nodiscard]] glm::vec2 GetGlobalPosition() const { return m_layout_result.global_position; }
        [[nodiscard]] glm::vec2 GetGlobalSize() const { return m_layout_result.global_size; }

        [[nodiscard]] glm::vec2 GetPivot() const { return m_pivot; }
        [[nodiscard]] Anchor GetAnchor() const { return m_anchor; }
        [[nodiscard]] std::optional<Ecs::EntityId> GetParent() const { return m_parent_entity; }

        [[nodiscard]] glm::mat4 GetModelMatrix() const { return m_layout_result.global_matrix; }
        [[nodiscard]] uint8_t GetLayer() const { return m_layout_result.layer; }

        RectTransform &SetPosition(const glm::vec2 local_position) {
            m_local_position = local_position;
            m_is_dirty = true;
            return *this;
        }

        RectTransform &SetSize(const glm::vec2 size) {
            m_size = size;
            m_size.x = glm::abs(size.x);
            m_size.y = glm::abs(size.y);
            m_is_dirty = true;
            return *this;
        }

        RectTransform &SetPivot(const glm::vec2 pivot) {
            m_pivot = pivot;
            m_pivot.x = glm::clamp(pivot.x, 0.0f, 1.0f);
            m_pivot.y = glm::clamp(pivot.y, 0.0f, 1.0f);

            m_is_dirty = true;
            return *this;
        }

        RectTransform &SetAnchor(const Anchor anchor) {
            m_anchor = anchor;
            m_is_dirty = true;
            return *this;
        }

        RectTransform &SetParent(const Ecs::EntityId parent_entity) {
            m_parent_entity = parent_entity;
            m_is_dirty = true;
            return *this;
        }

    private:
        glm::vec2 m_local_position = glm::vec2(0.0f);
        glm::vec2 m_size = glm::vec2(1.0f);
        glm::vec2 m_pivot = glm::vec2(0.5f, 0.5f);
        Anchor m_anchor = Anchor::TopLeft;
        std::optional<Ecs::EntityId> m_parent_entity;
        bool m_is_dirty = false;
        UiLayoutResult m_layout_result{};
    };
}
