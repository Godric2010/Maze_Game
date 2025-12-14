//
// Created by Sebastian Borsch on 28.10.25.
//

#pragma once
#include <algorithm>
#include <optional>
#include <glm/vec2.hpp>
#include "../ecs/include/World.hpp"

namespace Engine::Components::UI {
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

    struct RectTransform {
    public:
        RectTransform() = default;

        [[nodiscard]] glm::vec2 GetLocalPosition() const { return m_local_position; }
        [[nodiscard]] glm::vec2 GetLocalSize() const { return m_size; }

        [[nodiscard]] glm::vec2 GetPivot() const { return m_pivot; }
        [[nodiscard]] Anchor GetAnchor() const { return m_anchor; }
        [[nodiscard]] std::optional<Ecs::EntityId> GetParent() const { return m_parent_entity; }
        [[nodiscard]] uint64_t GetVersion() const { return m_version; }

        RectTransform &SetPosition(const glm::vec2 local_position) {
            m_local_position = local_position;
            m_version++;
            return *this;
        }

        RectTransform &SetSize(const glm::vec2 size) {
            m_size = size;
            m_size.x = abs(size.x);
            m_size.y = abs(size.y);
            m_version++;
            return *this;
        }

        RectTransform &SetPivot(const glm::vec2 pivot) {
            m_pivot = pivot;
            m_pivot.x = std::ranges::clamp(pivot.x, 0.0f, 1.0f);
            m_pivot.y = std::ranges::clamp(pivot.y, 0.0f, 1.0f);
            m_version++;
            return *this;
        }

        RectTransform &SetAnchor(const Anchor anchor) {
            m_anchor = anchor;
            m_version++;
            return *this;
        }

        RectTransform &SetParent(const Ecs::EntityId parent_entity) {
            m_parent_entity = parent_entity;
            m_version++;
            return *this;
        }

    private:
        glm::vec2 m_local_position = glm::vec2(0.0f);
        glm::vec2 m_size = glm::vec2(1.0f);
        glm::vec2 m_pivot = glm::vec2(0.5f, 0.5f);
        Anchor m_anchor = Anchor::TopLeft;
        std::optional<Ecs::EntityId> m_parent_entity;
        uint64_t m_version = 0;
    };
}
