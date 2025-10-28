#include "RenderSystem.hpp"

#include "GameWorld.hpp"
#include "Mesh.hpp"
#include "components/Camera.hpp"
#include "components/Transform.hpp"
#include "ui/Image.hpp"
#include "ui/RectTransform.hpp"

namespace Engine::Core::Systems {
    RenderSystem::RenderSystem() = default;

    RenderSystem::~RenderSystem() = default;

    void RenderSystem::Initialize() {
        const Renderer::RenderController *render_controller = ServiceLocator()->GetService<
            Renderer::RenderController>();
        m_render_controller = render_controller;
    }

    void RenderSystem::Run(float delta_time) {
        const auto [camera, cameraEntity] = GameWorld()->GetComponentsOfType<Components::Camera>()[0];
        const auto camera_transform = GameWorld()->GetComponent<Components::Transform>(cameraEntity);
        const auto camera_asset = CreateCameraAsset(camera, camera_transform);

        const std::vector<Renderer::MeshDrawAsset> mesh_draw_assets = CreateDrawAssets();
        const std::vector<Renderer::UiDrawAsset> ui_draw_assets = CreateUiDrawAssets();

        Renderer::DrawAssets draw_assets{};
        draw_assets.mesh_draw_assets = mesh_draw_assets;
        draw_assets.ui_draw_assets = ui_draw_assets;

        m_render_controller->BeginFrame(camera_asset);
        m_render_controller->SubmitFrame(draw_assets);
    }

    Renderer::CameraAsset RenderSystem::CreateCameraAsset(const Components::Camera *camera_component,
                                                          const Components::Transform *camera_transform) {
        const Renderer::CameraAsset camera_asset{
            .view = camera_component->view,
            .projection = camera_component->projection,
            .cameraPosition = glm::vec4(camera_transform->GetPosition(), 1.0f)
        };

        return camera_asset;
    }

    std::vector<Renderer::MeshDrawAsset> RenderSystem::CreateDrawAssets() const {
        const auto mesh_components = GameWorld()->GetComponentsOfType<Components::Mesh>();
        auto draw_assets = std::vector<Renderer::MeshDrawAsset>(mesh_components.size());

        for (size_t i = 0; i < mesh_components.size(); ++i) {
            const auto [mesh_component, meshEntity] = mesh_components[i];
            const auto mesh_transform = GameWorld()->GetComponent<Components::Transform>(meshEntity);

            Renderer::MeshDrawAsset mesh_draw_assets{};
            mesh_draw_assets.mesh = mesh_component->mesh;
            mesh_draw_assets.model = mesh_transform->GetMatrix();
            mesh_draw_assets.color = mesh_component->color;

            draw_assets[i] = mesh_draw_assets;
        }
        return draw_assets;
    }

    std::vector<Renderer::UiDrawAsset> RenderSystem::CreateUiDrawAssets() const {
        const auto rect_transforms = GameWorld()->GetComponentsOfType<Components::UI::RectTransform>();
        auto ui_draw_assets = std::vector<Renderer::UiDrawAsset>(rect_transforms.size());

        for (size_t i = 0; i < rect_transforms.size(); ++i) {
            const auto [rect_transform, entity] = rect_transforms[i];
            auto image_component = GameWorld()->GetComponent<Components::UI::Image>(entity);

            Renderer::UiDrawAsset ui_draw_asset{};
            ui_draw_asset.position = rect_transform->position;
            ui_draw_asset.size = rect_transform->size;
            ui_draw_asset.color = image_component->color;

            ui_draw_assets[i] = ui_draw_asset;
        }

        return ui_draw_assets;
    }
} // namespace
