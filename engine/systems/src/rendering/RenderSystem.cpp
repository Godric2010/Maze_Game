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
        m_draw_assets = Renderer::DrawAssets{};
    }

    void RenderSystem::Run(float delta_time)
    {
        const auto [camera, cameraEntity] = EcsWorld()->GetComponentsOfType<Components::Camera>()[0];
        const auto camera_transform = EcsWorld()->GetComponent<Components::Transform>(cameraEntity);
        const auto camera_asset = CreateCameraAsset(cameraEntity, camera_transform);

        ClearDrawAssets();
        FillMeshDrawAssets();
        FillUiDrawAssets();

        m_render_controller->BeginFrame(camera_asset);
        m_render_controller->SubmitFrame(m_draw_assets);
    }

    Renderer::CameraAsset RenderSystem::CreateCameraAsset(const Ecs::EntityId& camera_entity,
                                                          const Components::Transform* camera_transform) const
    {
        const auto camera_cache_val = Cache()->GetCameraCache()->GetCacheValue(camera_entity);
        const Renderer::CameraAsset camera_asset{
            .view = camera_cache_val.view,
            .projection = camera_cache_val.projection,
            .camera_position = glm::vec4(camera_transform->GetPosition(), 1.0f)
        };

        return camera_asset;
    }

    void RenderSystem::ClearDrawAssets()
    {
        m_draw_assets.mesh_draw_assets.clear();
        m_draw_assets.ui_draw_assets.clear();
    }

    void RenderSystem::FillMeshDrawAssets()
    {
        const auto mesh_renderer_components = EcsWorld()->GetComponentsOfType<Components::MeshRenderer>();
        auto& mesh_assets = m_draw_assets.mesh_draw_assets;
        mesh_assets.reserve(mesh_renderer_components.size());
        for (size_t i = 0; i < mesh_renderer_components.size(); ++i)
        {
            const auto [mesh_renderer, meshEntity] = mesh_renderer_components[i];

            Renderer::MeshDrawAsset mesh_draw_assets{};
            mesh_draw_assets.mesh = mesh_renderer->mesh;
            mesh_draw_assets.model = Cache()->GetTransformCache()->GetTransformValue(meshEntity).transform_matrix;
            mesh_draw_assets.texture = mesh_renderer->texture;
            mesh_draw_assets.color = mesh_renderer->color;

            mesh_assets.emplace_back(mesh_draw_assets);
        }
    }

    void RenderSystem::FillUiDrawAssets()
    {
        const auto rect_transforms = EcsWorld()->GetComponentsOfType<Components::UI::RectTransform>();
        auto& ui_draw_assets = m_draw_assets.ui_draw_assets;
        ui_draw_assets.reserve(rect_transforms.size());

        for (size_t i = 0; i < rect_transforms.size(); ++i)
        {
            const auto [rect_transform, entity] = rect_transforms[i];
            auto rect_transform_cache_value = Cache()->GetTransformCache()->GetRectTransformValue(entity);
            Renderer::UiDrawAsset ui_draw_asset{};
            ui_draw_asset.model = rect_transform_cache_value.global_matrix;
            ui_draw_asset.layer = rect_transform_cache_value.layer;

            const auto image_component = EcsWorld()->GetComponent<Components::UI::Image>(entity);
            if (image_component != nullptr)
            {
                ui_draw_asset.color = image_component->color;
                ui_draw_asset.mesh = static_cast<Renderer::MeshHandle>(0);
                ui_draw_assets.emplace_back(ui_draw_asset);
                continue;
            }

            const auto button_component = EcsWorld()->GetComponent<Components::UI::Button>(entity);
            if (button_component != nullptr)
            {
                ui_draw_asset.color = Cache()->GetUiCache()->GetButtonElement(entity).color;
                ui_draw_asset.mesh = static_cast<Renderer::MeshHandle>(0);
                ui_draw_assets.emplace_back(ui_draw_asset);
                continue;
            }

            const auto text_component = EcsWorld()->GetComponent<Components::UI::Text>(entity);
            if (text_component != nullptr)
            {
                auto text_element = Cache()->GetUiCache()->GetTextElement(entity);
                if (!text_element.texture_handle.has_value() || !text_element.text_mesh.has_value())
                {
                    continue;
                }

                ui_draw_asset.color = glm::vec4(1, 1, 1, 1);
                ui_draw_asset.mesh = text_element.text_mesh.value();
                ui_draw_asset.texture = text_element.texture_handle.value();
                ui_draw_assets.emplace_back(ui_draw_asset);
                continue;
            }
        }

        std::ranges::sort(ui_draw_assets,
                          [](auto& a, auto& b)
                          {
                              return a.layer < b.layer;
                          }
        );
    }
} // namespace
