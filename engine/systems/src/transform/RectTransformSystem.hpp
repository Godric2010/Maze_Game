#pragma once
#include <glm/vec2.hpp>
#include <ui/RectTransform.hpp>
#include "IEngineSystem.hpp"

namespace yarep::systems {
    ECS_SYSTEM(RectTransformSystem, LateUpdate, TAGS(ENGINE), DEPENDENCIES())

    struct LayoutData {
        math::Vec2 local_position;
        math::Vec2 local_size;
        math::Vec2 pivot;
        math::Vec2 anchor_point;
        uint8_t parent_layer;
    };

    class RectTransformSystem : public ecs::IEngineSystem {
    public:
        RectTransformSystem() = default;

        ~RectTransformSystem() override = default;

        void Initialize() override;

        void Run(float delta_time) override;

    private:
        static math::Vec2 GetAnchorValue(const components::ui::Anchor& anchor);

        LayoutData CreateLayoutData(const components::ui::RectTransform* rect_transform);

        static transform::RectTransformCacheValue CreateUiLayoutResult(const LayoutData& rect_layout);

        transform::RectTransformCacheValue GetParentLayoutResult(const ecs::EntityId& parent_entity);

        math::Vec2 m_world_origin = math::Vec2{};
        math::Vec2 m_world_scale = math::Vec2(1920, 1080);
    };
} // namespace
