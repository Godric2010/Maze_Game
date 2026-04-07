#pragma once
#include <IRenderController.hpp>
#include <IEngineSystem.hpp>
#include <Transform.hpp>

#include "MeshRenderer.hpp"

ECS_SYSTEM(RenderSystem,
           Render,
           TAGS(ENGINE),
           DEPENDENCIES(UiImageSystem, UiButtonSystem, UiTextSystem, RectTransformSystem, TransformSystem, CameraSystem
           )
        )

namespace Engine::Systems {
    class RenderSystem : public Ecs::IEngineSystem {
    public:
        RenderSystem();

        ~RenderSystem() override;

        void Initialize() override;

        void Run(float delta_time) override;

    private:
        const Renderer::IRenderController* m_render_controller{};
        const AssetHandling::AssetHandler* m_asset_handler{};
        std::vector<Renderer::DrawAsset> m_draw_assets;
        std::unordered_map<Ecs::EntityId, Renderer::DrawAsset> m_draw_asset_map;
        std::unordered_map<Ecs::EntityId, Renderer::DrawAsset> m_ui_draw_asset_map;
        std::unordered_map<Ecs::EntityId, Renderer::DrawAsset> m_ui_text_asset_map;

        Renderer::CameraAsset CreateCameraAsset(const Ecs::EntityId& camera_entity,
                                                const Components::Transform* camera_transform) const;

        void ClearDrawAssets();

        void FillMeshDrawAssets();

        bool IsDrawAssetValid(const Renderer::DrawAsset& ui_draw_asset) const;

        void FillUiDrawAssets();

        void RegisterDrawAssets(const Ecs::EntityId& entity, const Components::MeshRenderer& mesh_renderer);

        void RegisterColorUiAssets(const Ecs::EntityId& entity);

        void RegisterTextUiAssets(const Ecs::EntityId& entity);
    };
} // namespace
