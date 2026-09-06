#include "RenderSystem.hpp"

#include <Camera.hpp>
#include <Lights.hpp>
#include <MeshRenderer.hpp>
#include <TransformComponent.hpp>
#include <ui/Button.hpp>
#include <ui/Image.hpp>
#include <ui/RectTransform.hpp>
#include <ui/Text.hpp>


namespace yarep::systems {
    RenderSystem::RenderSystem() {
        m_ambient_light = renderer::AmbientLightAsset{
            .color = glm::vec3(1.0f, 1.0f, 1.0f),
            .intensity = 0.02f
        };
    }

    RenderSystem::~RenderSystem() = default;

    void RenderSystem::Initialize() {
        const auto* render_controller = ServiceLocator()->GetService<renderer::IRenderController>();
        m_render_controller = render_controller;
        const auto* asset_handler = ServiceLocator()->GetService<asset_handling::AssetHandler>();
        m_asset_handler = asset_handler;
        EcsWorld()->GetComponentEventBus()->SubscribeOnComponentAddEvent<components::MeshRenderer>(
                [this](const ecs::EntityId entity, const components::MeshRenderer& mesh_renderer) {
                    this->RegisterDrawAssets(entity, mesh_renderer);
                }
                );
        EcsWorld()->GetComponentEventBus()->SubscribeOnComponentAddEvent<components::ui::Image>(
                [this](const ecs::EntityId entity, const components::ui::Image& _) {
                    this->RegisterColorUiAssets(entity);
                }
                );
        EcsWorld()->GetComponentEventBus()->SubscribeOnComponentAddEvent<components::ui::Button>(
                [this](const ecs::EntityId entity, const components::ui::Button& _) {
                    this->RegisterColorUiAssets(entity);
                }
                );
        EcsWorld()->GetComponentEventBus()->SubscribeOnComponentAddEvent<components::ui::Text>(
                [this](const ecs::EntityId entity, const components::ui::Text& _) {
                    this->RegisterTextUiAssets(entity);
                }
                );
        EcsWorld()->GetComponentEventBus()->SubscribeOnComponentRemoveEvent<components::MeshRenderer>(
                [this](const ecs::EntityId entity) {
                    this->m_draw_asset_map.erase(entity);
                }
                );
        EcsWorld()->GetComponentEventBus()->SubscribeOnComponentRemoveEvent<components::ui::Image>(
                [this](const ecs::EntityId entity) {
                    this->m_ui_draw_asset_map.erase(entity);
                }
                );
        EcsWorld()->GetComponentEventBus()->SubscribeOnComponentRemoveEvent<components::ui::Button>(
                [this](const ecs::EntityId entity) {
                    this->m_ui_draw_asset_map.erase(entity);
                }
                );
        EcsWorld()->GetComponentEventBus()->SubscribeOnComponentRemoveEvent<components::ui::Text>(
                [this](const ecs::EntityId entity) {
                    this->m_ui_text_asset_map.erase(entity);
                }
                );
        m_draw_assets = std::vector<renderer::DrawAsset>();
    }

    void RenderSystem::Run(float delta_time) {
        const auto frame_data = FillFrameData();
        ClearDrawAssets();
        FillMeshDrawAssets();
        FillUiDrawAssets();
        m_render_controller->RenderFrame(frame_data, m_draw_assets);
    }

    renderer::CameraAsset RenderSystem::CreateCameraAsset(const ecs::EntityId& camera_entity,
                                                          const components::TransformComponent* camera_transform) const {
        const auto camera_cache_val = Cache()->GetCameraCache()->GetCacheValue(camera_entity);
        const renderer::CameraAsset camera_asset{
            .view = camera_cache_val.view,
            .projection = camera_cache_val.projection,
            .camera_position = glm::vec4(camera_transform->GetPosition(), 1.0f)
        };
        return camera_asset;
    }

    void RenderSystem::ClearDrawAssets() {
        m_draw_assets.clear();
        m_draw_assets.reserve(m_draw_asset_map.size() + m_ui_draw_asset_map.size() + m_ui_text_asset_map.size());
    }

    renderer::FrameData RenderSystem::FillFrameData() const {
        renderer::FrameData frame_data;
        frame_data.ambient_light = m_ambient_light;

        const auto [camera, cameraEntity] = EcsWorld()->GetComponentsOfType<components::Camera>()[0];
        const auto camera_transform = EcsWorld()->GetComponent<components::TransformComponent>(cameraEntity);
        const auto camera_asset = CreateCameraAsset(cameraEntity, camera_transform);
        frame_data.camera = camera_asset;

        const auto point_lights = EcsWorld()->GetComponentsOfType<components::PointLight>();
        frame_data.lights.reserve(point_lights.size());
        for (auto [point_light, entity]: point_lights) {
            const auto light_transform = EcsWorld()->GetComponent<components::TransformComponent>(entity);
            const auto light_asset = renderer::LightAsset{
                .position = light_transform->GetPosition(),
                .color = point_light->GetColor(),
                .intensity = point_light->GetIntensity(),
                .constant_attenuation = point_light->GetConstant(),
                .linear_attenuation = point_light->GetLinear(),
                .quadratic_attenuation = point_light->GetQuadratic()
            };
            frame_data.lights.emplace_back(light_asset);
        }
        return frame_data;
    }

    void RenderSystem::FillMeshDrawAssets() {
        for (auto [entity, mesh_draw_asset]: m_draw_asset_map) {
            if (!m_asset_handler->GetAsset<asset_handling::MeshAsset>(mesh_draw_asset.mesh)->IsValid()) {
                continue;
            }

            mesh_draw_asset.model = Cache()->GetTransformCache()->GetTransformValue(entity).transform_matrix;
            m_draw_assets.push_back(mesh_draw_asset);
        }
    }


    void RenderSystem::FillUiDrawAssets() {
        const auto transform_cache = Cache()->GetTransformCache();
        const auto ui_cache = Cache()->GetUiCache();
        for (auto [entity, ui_draw_asset]: m_ui_draw_asset_map) {
            if (!IsDrawAssetValid(ui_draw_asset)) {
                continue;
            }

            const auto rect_transform = transform_cache->GetRectTransformValue(entity);
            ui_draw_asset.model = rect_transform.global_matrix;
            ui_draw_asset.render_queue_index = rect_transform.layer;

            ui_draw_asset.color = ui_cache->GetColorElement(entity).color;
            m_draw_assets.push_back(ui_draw_asset);
        }

        for (auto [entity, ui_draw_asset]: m_ui_text_asset_map) {
            if (!IsDrawAssetValid(ui_draw_asset)) {
                continue;
            }

            const auto rect_transform = transform_cache->GetRectTransformValue(entity);
            ui_draw_asset.model = rect_transform.global_matrix;
            ui_draw_asset.render_queue_index = rect_transform.layer;

            m_draw_assets.push_back(ui_draw_asset);
        }
    }

    void RenderSystem::RegisterDrawAssets(const ecs::EntityId& entity, const components::MeshRenderer& mesh_renderer) {
        const auto material = m_asset_handler->GetAsset<asset_handling::MaterialAsset>(mesh_renderer.material);
        if (material == nullptr) {
            throw std::runtime_error("[RenderSystem] Material not found");
        }
        const renderer::DrawAsset mesh_draw_assets{
            .entity = entity,
            .render_state = material->render_state,
            .render_queue_index = 0,
            .mesh = mesh_renderer.mesh,
            .material = mesh_renderer.material,
            .color = material->base_color,
        };
        m_draw_asset_map[entity] = mesh_draw_assets;
    }

    void RenderSystem::RegisterColorUiAssets(const ecs::EntityId& entity) {
        const auto color_element = Cache()->GetUiCache()->GetColorElement(entity);
        renderer::DrawAsset draw_asset{};
        draw_asset.entity = entity;
        draw_asset.render_state = asset_handling::RenderState::UI;
        draw_asset.color = color_element.color;
        draw_asset.mesh = color_element.mesh_handle;
        draw_asset.material = color_element.material_handle;
        m_ui_draw_asset_map[entity] = draw_asset;
    }

    void RenderSystem::RegisterTextUiAssets(const ecs::EntityId& entity) {
        const auto text_element = Cache()->GetUiCache()->GetTextElement(entity);
        renderer::DrawAsset draw_asset{};
        draw_asset.render_state = asset_handling::RenderState::UI;
        draw_asset.entity = entity;
        draw_asset.color = glm::vec4(1, 1, 1, 1);
        draw_asset.mesh = text_element.mesh_handle;
        draw_asset.material = text_element.material_handle;
        m_ui_text_asset_map[entity] = draw_asset;
    }

    bool RenderSystem::IsDrawAssetValid(const renderer::DrawAsset& ui_draw_asset) const {
        if (!ui_draw_asset.material || !ui_draw_asset.mesh) {
            return false;
        }

        if (!m_asset_handler->GetAsset<asset_handling::MeshAsset>(ui_draw_asset.mesh)->IsValid()) {
            return false;
        }
        return true;
    }
} // namespace
