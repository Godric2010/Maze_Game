//
// Created by Sebastian Borsch on 28.10.25.
//

#pragma once
#include <algorithm>
#include <optional>
#include <Math.hpp>
#include "Ecs/Types.hpp"

namespace yarep::components::ui {
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

        [[nodiscard]] math::Vec2 GetLocalPosition() const { return m_local_position; }
        [[nodiscard]] math::Vec2 GetLocalSize() const { return m_size; }

        [[nodiscard]] math::Vec2 GetPivot() const { return m_pivot; }
        [[nodiscard]] Anchor GetAnchor() const { return m_anchor; }
        [[nodiscard]] std::optional<ecs::EntityId> GetParent() const { return m_parent_entity; }
        [[nodiscard]] uint64_t GetVersion() const { return m_version; }

        RectTransform &SetPosition(const math::Vec2 local_position) {
            m_local_position = local_position;
            m_version++;
            return *this;
        }

        RectTransform &SetSize(const math::Vec2 size) {
            m_size = size;
            m_size.x = abs(size.x);
            m_size.y = abs(size.y);
            m_version++;
            return *this;
        }

        RectTransform &SetPivot(const math::Vec2 pivot) {
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

        RectTransform &SetParent(const ecs::EntityId parent_entity) {
            m_parent_entity = parent_entity;
            m_version++;
            return *this;
        }

    private:
        math::Vec2 m_local_position = math::Vec2{};
        math::Vec2 m_size = math::Vec2{1.f, 1.f};
        math::Vec2 m_pivot = math::Vec2(0.5f, 0.5f);
        Anchor m_anchor = Anchor::TopLeft;
        std::optional<ecs::EntityId> m_parent_entity;
        uint64_t m_version = 0;
    };
}
