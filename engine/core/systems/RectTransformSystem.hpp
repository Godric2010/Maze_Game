#pragma once
#include <glm/vec2.hpp>

#include "ISystem.hpp"
#include "../../components/ui/RectTransform.hpp"

namespace Engine::Core::Systems {
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
        static glm::vec2 GetAnchorValue(const Components::UI::Anchor &anchor);

        LayoutData CreateLayoutData(const Components::UI::RectTransform* rect_transform);

        static Components::UI::UiLayoutResult CreateUiLayoutResult(const LayoutData &rect_layout);

        Components::UI::UiLayoutResult GetParentLayoutResult(const Ecs::EntityId &entity);

        std::unordered_map<Ecs::EntityId, Components::UI::RectTransform *> m_dirty_rect_transforms;
        std::unordered_map<Ecs::EntityId, Components::UI::UiLayoutResult> m_calculated_layouts;

        glm::vec2 m_world_origin = glm::vec2(0.0f);
        glm::vec2 m_world_scale = glm::vec2(1920, 1080);
    };
} // namespace
