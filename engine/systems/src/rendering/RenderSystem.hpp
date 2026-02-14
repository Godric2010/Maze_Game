#pragma once
#include <IRenderController.hpp>
#include <IEngineSystem.hpp>
#include <Transform.hpp>

ECS_SYSTEM(RenderSystem, Render, [ENGINE])

namespace Engine::Systems
{
    class RenderSystem : public Ecs::IEngineSystem
    {
    public:
        RenderSystem();

        ~RenderSystem() override;

        void Initialize() override;

        void Run(float delta_time) override;

    private:
        const Renderer::IRenderController* m_render_controller{};
        Renderer::DrawAssets m_draw_assets;

        Renderer::CameraAsset CreateCameraAsset(const Ecs::EntityId& camera_entity,
                                                const Components::Transform* camera_transform) const;

        void ClearDrawAssets();

        [[nodiscard]] void FillMeshDrawAssets();

        [[nodiscard]] void FillUiDrawAssets();
    };
} // namespace
