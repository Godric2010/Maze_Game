#pragma once
#include <IRenderController.hpp>
#include <IEngineSystem.hpp>
#include <TransformComponent.hpp>

#include "MeshRenderer.hpp"

ECS_SYSTEM(RenderSystem,
           Render,
           TAGS(ENGINE),
           DEPENDENCIES(UiImageSystem, UiButtonSystem, UiTextSystem, RectTransformSystem, TransformSystem, CameraSystem
           )
        )

namespace yarep::systems {
    class RenderSystem : public ecs::IEngineSystem {
    public:
        RenderSystem();

        ~RenderSystem() override;

        void Initialize() override;

        void Run(float delta_time) override;

    private:
        const renderer::IRenderController* m_render_controller{};
        const asset_handling::AssetHandler* m_asset_handler{};
        std::vector<renderer::DrawAsset> m_draw_assets;
        std::unordered_map<ecs::EntityId, renderer::DrawAsset> m_draw_asset_map;
        std::unordered_map<ecs::EntityId, renderer::DrawAsset> m_ui_draw_asset_map;
        std::unordered_map<ecs::EntityId, renderer::DrawAsset> m_ui_text_asset_map;
        renderer::AmbientLightAsset m_ambient_light{};

        renderer::CameraAsset CreateCameraAsset(const ecs::EntityId& camera_entity,
                                                const components::TransformComponent* camera_transform) const;

        void ClearDrawAssets();

        renderer::FrameData FillFrameData() const;

        void FillMeshDrawAssets();

        bool IsDrawAssetValid(const renderer::DrawAsset& ui_draw_asset) const;

        void FillUiDrawAssets();

        void RegisterDrawAssets(const ecs::EntityId& entity, const components::MeshRenderer& mesh_renderer);

        void RegisterColorUiAssets(const ecs::EntityId& entity);

        void RegisterTextUiAssets(const ecs::EntityId& entity);
    };
} // namespace
