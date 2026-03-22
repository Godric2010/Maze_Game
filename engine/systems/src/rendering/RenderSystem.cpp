#include "RenderSystem.hpp"

#include <Camera.hpp>
#include <MeshRenderer.hpp>
#include <Transform.hpp>
#include <ui/Button.hpp>
#include <ui/Image.hpp>
#include <ui/RectTransform.hpp>
#include <ui/Text.hpp>

namespace Engine::Systems
{
    RenderSystem::RenderSystem() = default;
    RenderSystem::~RenderSystem() = default;

    void RenderSystem::Initialize()
    {
        const auto* render_controller = ServiceLocator()->GetService<Renderer::IRenderController>();
        m_render_controller = render_controller;
        const auto* asset_handler = ServiceLocator()->GetService<AssetHandling::AssetHandler>();
        m_asset_handler = asset_handler;
        EcsWorld()->GetComponentEventBus()->SubscribeOnComponentAddEvent<Components::MeshRenderer>(
            [this](const Ecs::EntityId entity, const Components::MeshRenderer& mesh_renderer)
            {
                this->RegisterDrawAssets(entity, mesh_renderer);
            });
        EcsWorld()->GetComponentEventBus()->SubscribeOnComponentAddEvent<Components::UI::Image>(
            [this](const Ecs::EntityId entity, const Components::UI::Image& _)
            {
                this->RegisterColorUiAssets(entity);
            });
        EcsWorld()->GetComponentEventBus()->SubscribeOnComponentAddEvent<Components::UI::Button>(
            [this](const Ecs::EntityId entity, const Components::UI::Button& _)
            {
                this->RegisterColorUiAssets(entity);
            });
        EcsWorld()->GetComponentEventBus()->SubscribeOnComponentAddEvent<Components::UI::Text>(
            [this](const Ecs::EntityId entity, const Components::UI::Text& _)
            {
                this->RegisterTextUiAssets(entity);
            });
        EcsWorld()->GetComponentEventBus()->SubscribeOnComponentRemoveEvent<Components::MeshRenderer>(
            [this](const Ecs::EntityId entity)
            {
                this->m_draw_asset_map.erase(entity);
            });
        EcsWorld()->GetComponentEventBus()->SubscribeOnComponentRemoveEvent<Components::UI::Image>(
            [this](const Ecs::EntityId entity)
            {
                this->m_ui_draw_asset_map.erase(entity);
            });
        EcsWorld()->GetComponentEventBus()->SubscribeOnComponentRemoveEvent<Components::UI::Button>(
            [this](const Ecs::EntityId entity)
            {
                this->m_ui_draw_asset_map.erase(entity);
            });
        EcsWorld()->GetComponentEventBus()->SubscribeOnComponentRemoveEvent<Components::UI::Text>(
            [this](const Ecs::EntityId entity)
            {
                this->m_ui_text_asset_map.erase(entity);
            });
        m_draw_assets = std::vector<Renderer::DrawAsset>();
    }

    void RenderSystem::Run(float delta_time)
    {
        const auto [camera, cameraEntity] = EcsWorld()->GetComponentsOfType<Components::Camera>()[0];
        const auto camera_transform = EcsWorld()->GetComponent<Components::Transform>(cameraEntity);
        const auto camera_asset = CreateCameraAsset(cameraEntity, camera_transform);
        ClearDrawAssets();
        FillMeshDrawAssets();
        FillUiDrawAssets();
        m_render_controller->RenderFrame(camera_asset, m_draw_assets);
    }

    Renderer::CameraAsset RenderSystem::CreateCameraAsset(const Ecs::EntityId& camera_entity,
                                                          const Components::Transform* camera_transform) const
    {
        const auto camera_cache_val = Cache()->GetCameraCache()->GetCacheValue(camera_entity);
        const Renderer::CameraAsset camera_asset{
            .view = camera_cache_val.view, .projection = camera_cache_val.projection,
            .camera_position = glm::vec4(camera_transform->GetPosition(), 1.0f)
        };
        return camera_asset;
    }

    void RenderSystem::ClearDrawAssets()
    {
        m_draw_assets.clear();
        m_draw_assets.reserve(m_draw_asset_map.size() + m_ui_draw_asset_map.size() + m_ui_text_asset_map.size());
    }

    void RenderSystem::FillMeshDrawAssets()
    {
        for (auto [entity, mesh_draw_asset] : m_draw_asset_map)
        {
            if (!m_asset_handler->GetAsset<AssetHandling::MeshAsset>(mesh_draw_asset.Mesh)->IsValid())
            {
                continue;
            }

            mesh_draw_asset.Model = Cache()->GetTransformCache()->GetTransformValue(entity).transform_matrix;
            m_draw_assets.push_back(mesh_draw_asset);
        }
    }


    void RenderSystem::FillUiDrawAssets()
    {
        const auto transform_cache = Cache()->GetTransformCache();
        const auto ui_cache = Cache()->GetUiCache();
        for (auto [entity, ui_draw_asset] : m_ui_draw_asset_map)
        {
            if (!IsDrawAssetValid(ui_draw_asset))
            {
                continue;
            }

            const auto rect_transform = transform_cache->GetRectTransformValue(entity);
            ui_draw_asset.Model = rect_transform.global_matrix;
            ui_draw_asset.RenderQueueIndex = rect_transform.layer;

            ui_draw_asset.Color = ui_cache->GetColorElement(entity).color;
            m_draw_assets.push_back(ui_draw_asset);
        }
        
        for (auto [entity, ui_draw_asset] : m_ui_text_asset_map)
        {
            if (!IsDrawAssetValid(ui_draw_asset))
            {
                continue;
            }

            const auto rect_transform = transform_cache->GetRectTransformValue(entity);
            ui_draw_asset.Model = rect_transform.global_matrix;
            ui_draw_asset.RenderQueueIndex = rect_transform.layer;

            m_draw_assets.push_back(ui_draw_asset);
        }
        
    }

    void RenderSystem::RegisterDrawAssets(const Ecs::EntityId& entity, const Components::MeshRenderer& mesh_renderer)
    {
        const auto material = m_asset_handler->GetAsset<AssetHandling::MaterialAsset>(mesh_renderer.Material);
        if (material == nullptr)
        {
            throw std::runtime_error("[RenderSystem] Material not found");
        }
        const Renderer::DrawAsset mesh_draw_assets{
            .Entity = entity,
            .RenderState = material->render_state,
            .RenderQueueIndex = 0,
            .Mesh = mesh_renderer.Mesh,
            .Material = mesh_renderer.Material,
            .Color = material->base_color,
        };
        m_draw_asset_map[entity] = mesh_draw_assets;
    }

    void RenderSystem::RegisterColorUiAssets(const Ecs::EntityId& entity)
    {
        const auto color_element = Cache()->GetUiCache()->GetColorElement(entity);
        Renderer::DrawAsset draw_asset{};
        draw_asset.Entity = entity;
        draw_asset.RenderState = AssetHandling::RenderState::UI;
        draw_asset.Color = color_element.color;
        draw_asset.Mesh = color_element.mesh_handle;
        draw_asset.Material = color_element.material_handle;
        m_ui_draw_asset_map[entity] = draw_asset;
    }

    void RenderSystem::RegisterTextUiAssets(const Ecs::EntityId& entity)
    {
        const auto text_element = Cache()->GetUiCache()->GetTextElement(entity);
        Renderer::DrawAsset draw_asset{};
        draw_asset.RenderState = AssetHandling::RenderState::UI;
        draw_asset.Entity = entity;
        draw_asset.Color = glm::vec4(1, 1, 1, 1);
        draw_asset.Mesh = text_element.mesh_handle;
        draw_asset.Material = text_element.material_handle;
        m_ui_text_asset_map[entity] = draw_asset;
    }

    bool RenderSystem::IsDrawAssetValid(const Renderer::DrawAsset& ui_draw_asset) const
    {
        if (!ui_draw_asset.Material || !ui_draw_asset.Mesh)
        {
            return false;
        }

        if (!m_asset_handler->GetAsset<AssetHandling::MeshAsset>(ui_draw_asset.Mesh)->IsValid())
        {
            return false;
        }
        return true;
    }
} // namespace
