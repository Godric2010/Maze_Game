#pragma once
#include <IRenderController.hpp>
#include <IEngineSystem.hpp>
#include <Transform.hpp>

#include "MeshRenderer.hpp"

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
        const AssetHandling::AssetHandler* m_asset_handler{};
        Renderer::DrawAssets m_draw_assets;
        std::unordered_map<Ecs::EntityId, Renderer::MeshDrawAsset> m_draw_asset_map;

        Renderer::CameraAsset CreateCameraAsset(const Ecs::EntityId& camera_entity,
                                                const Components::Transform* camera_transform) const;

        void ClearDrawAssets();

        void FillMeshDrawAssets();

        void FillUiDrawAssets();

        void RegisterDrawAssets(const Ecs::EntityId& entity, const Components::MeshRenderer& mesh_renderer);
    };
} // namespace
