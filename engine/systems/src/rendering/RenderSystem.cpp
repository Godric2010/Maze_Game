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
            .color = math::Vec3(1.0f, 1.0f, 1.0f),
            .intensity = 0.02f
        };
    }

    RenderSystem::~RenderSystem() = default;

    void RenderSystem::Initialize() {
        const auto *render_controller = ServiceLocator()->GetService<renderer::IRenderController>();
        m_render_controller = render_controller;
        const auto *asset_handler = ServiceLocator()->GetService<asset_handling::AssetHandler>();
        m_asset_handler = asset_handler;
        EcsWorld()->GetComponentEventBus()->SubscribeOnComponentAddEvent<components::MeshRenderer>(
            [this](const ecs::EntityId entity, const components::MeshRenderer &mesh_renderer) {
                this->RegisterDrawAssets(entity, mesh_renderer);
            }
        );
        EcsWorld()->GetComponentEventBus()->SubscribeOnComponentAddEvent<components::ui::Image>(
            [this](const ecs::EntityId entity, const components::ui::Image &_) {
                this->RegisterColorUiAssets(entity);
            }
        );
        EcsWorld()->GetComponentEventBus()->SubscribeOnComponentAddEvent<components::ui::Button>(
            [this](const ecs::EntityId entity, const components::ui::Button &_) {
                this->RegisterColorUiAssets(entity);
            }
        );
        EcsWorld()->GetComponentEventBus()->SubscribeOnComponentAddEvent<components::ui::Text>(
            [this](const ecs::EntityId entity, const components::ui::Text &_) {
                this->RegisterTextUiAssets(entity);
            }
        );

        EcsWorld()->GetComponentEventBus()->SubscribeOnComponentAddEvent<components::PointLight>(
            [this](const ecs::EntityId entity, const components::PointLight &light) {
                this->RegisterLightAssets(entity, light);
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
        EcsWorld()->GetComponentEventBus()->SubscribeOnComponentRemoveEvent<components::PointLight>(
            [this](const ecs::EntityId entity) {
                this->m_light_asset_map.erase(entity);
            });
        m_draw_assets = std::vector<renderer::DrawAsset>();
        m_light_assets = std::vector<renderer::LightAsset>();
    }

    void RenderSystem::Run(float delta_time) {
        const auto frame_data = FillFrameData();
        ClearDrawAssets();
        ClearLightAssets();
        FillMeshDrawAssets();
        FillUiDrawAssets();
        m_render_controller->RenderFrame(frame_data, m_draw_assets);
    }

    renderer::CameraAsset RenderSystem::CreateCameraAsset(const ecs::EntityId &camera_entity,
                                                          const components::TransformComponent *camera_transform)
    const {
        const auto camera_cache_val = Cache()->GetCameraCache()->GetCacheValue(camera_entity);
        const auto cam_pos = camera_transform->GetPosition();
        const renderer::CameraAsset camera_asset{
            .view = camera_cache_val.view,
            .projection = camera_cache_val.projection,
            .camera_position = math::Vec4(cam_pos.x, cam_pos.y, cam_pos.z, 1.0f)
        };
        return camera_asset;
    }

    void RenderSystem::ClearDrawAssets() {
        m_draw_assets.clear();
        m_draw_assets.reserve(m_draw_asset_map.size() + m_ui_draw_asset_map.size() + m_ui_text_asset_map.size());
    }

    void RenderSystem::ClearLightAssets() {
        m_light_assets.clear();
        m_light_assets.reserve(m_light_asset_map.size());
    }

    renderer::FrameData RenderSystem::FillFrameData() {
        renderer::FrameData frame_data;
        frame_data.ambient_light = m_ambient_light;

        const auto [camera, cameraEntity] = EcsWorld()->GetComponentsOfType<components::Camera>()[0];
        const auto camera_transform = EcsWorld()->GetComponent<components::TransformComponent>(cameraEntity);
        const auto camera_asset = CreateCameraAsset(cameraEntity, camera_transform);
        frame_data.camera = camera_asset;

        FillLightDrawAssets();
        frame_data.lights = m_light_assets;
        return frame_data;
    }

    void RenderSystem::FillMeshDrawAssets() {
        for (auto [entity, mesh_draw_asset]: m_draw_asset_map) {
            const auto mesh_asset = m_asset_handler->GetAsset<asset_handling::MeshAsset>(mesh_draw_asset.mesh);
            if (!mesh_asset->IsValid()) {
                continue;
            }
            const auto transform_matrix = Cache()->GetTransformCache()->GetTransformValue(entity).transform_matrix;

            mesh_draw_asset.model = transform_matrix;
            mesh_draw_asset.world_aabb = geometry::transform_bounds(mesh_asset->local_aabb, transform_matrix);
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

    void RenderSystem::FillLightDrawAssets() {
        for (auto [entity, light_asset]: m_light_asset_map) {
            const auto transform_position = Cache()->GetTransformCache()->GetTransformValue(entity).last_position;
            light_asset.position = transform_position;
            m_light_assets.push_back(light_asset);
        }
    }

    void RenderSystem::RegisterDrawAssets(const ecs::EntityId &entity, const components::MeshRenderer &mesh_renderer) {
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

    void RenderSystem::RegisterColorUiAssets(const ecs::EntityId &entity) {
        const auto color_element = Cache()->GetUiCache()->GetColorElement(entity);
        renderer::DrawAsset draw_asset{};
        draw_asset.entity = entity;
        draw_asset.render_state = asset_handling::RenderState::UI;
        draw_asset.color = color_element.color;
        draw_asset.mesh = color_element.mesh_handle;
        draw_asset.material = color_element.material_handle;
        m_ui_draw_asset_map[entity] = draw_asset;
    }

    void RenderSystem::RegisterTextUiAssets(const ecs::EntityId &entity) {
        const auto text_element = Cache()->GetUiCache()->GetTextElement(entity);
        renderer::DrawAsset draw_asset{};
        draw_asset.render_state = asset_handling::RenderState::UI;
        draw_asset.entity = entity;
        draw_asset.color = math::Vec4(1, 1, 1, 1);
        draw_asset.mesh = text_element.mesh_handle;
        draw_asset.material = text_element.material_handle;
        m_ui_text_asset_map[entity] = draw_asset;
    }

    void RenderSystem::RegisterLightAssets(const ecs::EntityId &entity, const components::PointLight &light) {
        const auto light_asset = renderer::LightAsset{
            .color = light.GetColor(),
            .intensity = light.GetIntensity(),
            .constant_attenuation = light.GetConstant(),
            .linear_attenuation = light.GetLinear(),
            .quadratic_attenuation = light.GetQuadratic()
        };
        m_light_asset_map[entity] = light_asset;
    }


    bool RenderSystem::IsDrawAssetValid(const renderer::DrawAsset &ui_draw_asset) const {
        if (!ui_draw_asset.material || !ui_draw_asset.mesh) {
            return false;
        }

        if (!m_asset_handler->GetAsset<asset_handling::MeshAsset>(ui_draw_asset.mesh)->IsValid()) {
            return false;
        }
        return true;
    }
} // namespace
