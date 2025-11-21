#pragma once
#include <glm/vec2.hpp>

#include "ISystem.hpp"
#include "ui/RectTransform.hpp"

namespace Engine::Core::Systems {
    ECS_SYSTEM(RectTransformSystem, LateUpdate, [ENGINE])

    class RectTransformSystem : public Ecs::IEngineSystem {
    public:
        RectTransformSystem() = default;

        ~RectTransformSystem() override = default;

        void Initialize() override;

        void Run(float delta_time) override;

    private:
        static glm::vec2 GetAnchorValue(const Components::UI::Anchor &anchor);

        Components::UI::UiLayoutResult CreateUiLayoutResult(const Components::UI::RectTransform &rect_transform);

        glm::vec2 m_world_origin = glm::vec2(0.0f);
        glm::vec2 m_world_scale = glm::vec2(1920, 1080);
    };
} // namespace
