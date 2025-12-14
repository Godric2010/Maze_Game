#pragma once
#include <glm/vec2.hpp>

#include <ui/RectTransform.hpp>
#include "ISystem.hpp"


namespace Engine::Systems {
    ECS_SYSTEM(RectTransformSystem, LateUpdate, [ENGINE])

    struct LayoutData {
        glm::vec2 local_position;
        glm::vec2 local_size;
        glm::vec2 pivot;
        glm::vec2 anchor_point;
        uint8_t parent_layer;
    };

    class RectTransformSystem : public Ecs::IEngineSystem {
    public:
        RectTransformSystem() = default;

        ~RectTransformSystem() override = default;

        void Initialize() override;

        void Run(float delta_time) override;

    private:
        static glm::vec2 GetAnchorValue(const Components::UI::Anchor& anchor);

        LayoutData CreateLayoutData(const Components::UI::RectTransform* rect_transform);

        static Transform::RectTransformCacheValue CreateUiLayoutResult(const LayoutData& rect_layout);

        Transform::RectTransformCacheValue GetParentLayoutResult(const Ecs::EntityId& parent_entity);

        Transform::TransformCache* m_transform_cache;

        glm::vec2 m_world_origin = glm::vec2(0.0f);
        glm::vec2 m_world_scale = glm::vec2(1920, 1080);
    };
} // namespace
