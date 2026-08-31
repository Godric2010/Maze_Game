#pragma once
#include <glm/vec2.hpp>
#include <ui/RectTransform.hpp>
#include "IEngineSystem.hpp"

namespace yarep::systems {
    ECS_SYSTEM(RectTransformSystem, LateUpdate, TAGS(ENGINE), DEPENDENCIES())

    struct LayoutData {
        glm::vec2 local_position;
        glm::vec2 local_size;
        glm::vec2 pivot;
        glm::vec2 anchor_point;
        uint8_t parent_layer;
    };

    class RectTransformSystem : public ecs::IEngineSystem {
    public:
        RectTransformSystem() = default;

        ~RectTransformSystem() override = default;

        void Initialize() override;

        void Run(float delta_time) override;

    private:
        static glm::vec2 GetAnchorValue(const components::ui::Anchor& anchor);

        LayoutData CreateLayoutData(const components::ui::RectTransform* rect_transform);

        static transform::RectTransformCacheValue CreateUiLayoutResult(const LayoutData& rect_layout);

        transform::RectTransformCacheValue GetParentLayoutResult(const ecs::EntityId& parent_entity);

        glm::vec2 m_world_origin = glm::vec2(0.0f);
        glm::vec2 m_world_scale = glm::vec2(1920, 1080);
    };
} // namespace
