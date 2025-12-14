#include "RenderSystem.hpp"

#include <Camera.hpp>
#include <Mesh.hpp>
#include <Transform.hpp>
#include <ui/Button.hpp>
#include <ui/Image.hpp>
#include <ui/RectTransform.hpp>
#include <ui/Text.hpp>

namespace Engine::Systems {
    RenderSystem::RenderSystem() = default;

    RenderSystem::~RenderSystem() = default;

    void RenderSystem::Initialize() {
        const auto* render_controller = ServiceLocator()->GetService<Renderer::IRenderController>();
        m_render_controller = render_controller;
    }

    void RenderSystem::Run(float delta_time) {
        const auto [camera, cameraEntity] = EcsWorld()->GetComponentsOfType<Components::Camera>()[0];
        const auto camera_transform = EcsWorld()->GetComponent<Components::Transform>(cameraEntity);
        const auto camera_asset = CreateCameraAsset(cameraEntity, camera_transform);

        const std::vector<Renderer::MeshDrawAsset> mesh_draw_assets = CreateDrawAssets();
        const std::vector<Renderer::UiDrawAsset> ui_draw_assets = CreateUiDrawAssets();

        Renderer::DrawAssets draw_assets{};
        draw_assets.mesh_draw_assets = mesh_draw_assets;
        draw_assets.ui_draw_assets = ui_draw_assets;

        m_render_controller->BeginFrame(camera_asset);
        m_render_controller->SubmitFrame(draw_assets);
    }

    Renderer::CameraAsset RenderSystem::CreateCameraAsset(const Ecs::EntityId& camera_entity,
                                                          const Components::Transform* camera_transform) const {
        const auto camera_cache_val = Cache()->GetCameraCache()->GetCacheValue(camera_entity);
        const Renderer::CameraAsset camera_asset{
            .view = camera_cache_val.view,
            .projection = camera_cache_val.projection,
            .cameraPosition = glm::vec4(camera_transform->GetPosition(), 1.0f)
        };

        return camera_asset;
    }

    std::vector<Renderer::MeshDrawAsset> RenderSystem::CreateDrawAssets() const {
        const auto mesh_components = EcsWorld()->GetComponentsOfType<Components::Mesh>();
        auto draw_assets = std::vector<Renderer::MeshDrawAsset>(mesh_components.size());

        for (size_t i = 0; i < mesh_components.size(); ++i) {
            const auto [mesh_component, meshEntity] = mesh_components[i];

            Renderer::MeshDrawAsset mesh_draw_assets{};
            mesh_draw_assets.mesh = mesh_component->mesh;
            mesh_draw_assets.model = Cache()->GetTransformCache()->GetTransformValue(meshEntity).transform_matrix;
            mesh_draw_assets.color = mesh_component->color;

            draw_assets[i] = mesh_draw_assets;
        }
        return draw_assets;
    }

    std::vector<Renderer::UiDrawAsset> RenderSystem::CreateUiDrawAssets() const {
        const auto rect_transforms = EcsWorld()->GetComponentsOfType<Components::UI::RectTransform>();
        auto ui_draw_assets = std::vector<Renderer::UiDrawAsset>(rect_transforms.size());

        for (size_t i = 0; i < rect_transforms.size(); ++i) {
            const auto [rect_transform, entity] = rect_transforms[i];
            auto rect_transform_cache_value = Cache()->GetTransformCache()->GetRectTransformValue(entity);
            Renderer::UiDrawAsset ui_draw_asset{};
            ui_draw_asset.model = rect_transform_cache_value.global_matrix;
            ui_draw_asset.layer = rect_transform_cache_value.layer;

            const auto image_component = EcsWorld()->GetComponent<Components::UI::Image>(entity);
            if (image_component != nullptr) {
                ui_draw_asset.color = image_component->color;
                ui_draw_asset.mesh = static_cast<Renderer::MeshHandle>(0);
            }

            const auto button_component = EcsWorld()->GetComponent<Components::UI::Button>(entity);
            if (button_component != nullptr) {
                ui_draw_asset.color = Cache()->GetUiCache()->GetButtonElement(entity).color;
                ui_draw_asset.mesh = static_cast<Renderer::MeshHandle>(0);
            }

            const auto text_component = EcsWorld()->GetComponent<Components::UI::Text>(entity);
            if (text_component != nullptr) {
                auto text_element = Cache()->GetUiCache()->GetTextElement(entity);
                if (!text_element.texture_handle.has_value() || !text_element.text_mesh.has_value()) {
                    continue;
                }

                ui_draw_asset.color = glm::vec4(1, 1, 1, 1);
                ui_draw_asset.mesh = Cache()->GetUiCache()->GetTextElement(entity).text_mesh.value();
                ui_draw_asset.texture = Cache()->GetUiCache()->GetTextElement(entity).texture_handle.value();
            }

            ui_draw_assets[i] = ui_draw_asset;
        }

        std::ranges::sort(ui_draw_assets,
                          [](auto& a, auto& b) {
                              return a.layer < b.layer;
                          }
                );

        return ui_draw_assets;
    }
} // namespace
