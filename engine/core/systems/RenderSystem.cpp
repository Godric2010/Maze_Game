#include "RenderSystem.hpp"

#include "GameWorld.hpp"
#include "Mesh.hpp"
#include "components/Camera.hpp"
#include "components/Transform.hpp"

namespace Engine::Core::Systems {
    RenderSystem::RenderSystem() = default;

    RenderSystem::~RenderSystem() = default;

    void RenderSystem::Initialize() {
        const Renderer::RenderController *render_controller = ServiceLocator()->GetService<
            Renderer::RenderController>();
        m_renderController = render_controller;
    }

    void RenderSystem::Run(float delta_time) {
        const auto [camera, cameraEntity] = GameWorld()->GetComponentsOfType<Components::Camera>()[0];
        const auto camera_transform = GameWorld()->GetComponent<Components::Transform>(cameraEntity);
        const auto camera_asset = CreateCameraAsset(camera, camera_transform);

        const auto mesh_components = GameWorld()->GetComponentsOfType<Components::Mesh>();
        auto draw_data = std::vector<std::pair<Components::Mesh *, Components::Transform *> >(mesh_components.size());
        for (size_t i = 0; i < mesh_components.size(); ++i) {
            const auto [mesh, meshEntity] = mesh_components[i];
            auto mesh_transform = GameWorld()->GetComponent<Components::Transform>(meshEntity);
            const auto mesh_data = std::make_pair(mesh, mesh_transform);
            draw_data[i] = mesh_data;
        }
        const std::vector<Renderer::MeshDrawAsset> mesh_draw_assets = CreateDrawAssets(draw_data);

        Renderer::DrawAssets draw_assets{};
        draw_assets.mesh_draw_assets = mesh_draw_assets;
        draw_assets.ui_draw_assets = std::vector<Renderer::UiDrawAsset>();


        m_renderController->BeginFrame(camera_asset);
        m_renderController->SubmitFrame(draw_assets);
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

    std::vector<Renderer::MeshDrawAsset> RenderSystem::CreateDrawAssets(
        const std::vector<std::pair<Components::Mesh *, Components::Transform *> > &draw_data) {
        auto draw_assets = std::vector<Renderer::MeshDrawAsset>(draw_data.size());
        for (size_t i = 0; i < draw_data.size(); ++i) {
            draw_assets[i] = Renderer::MeshDrawAsset{
                .mesh = draw_data[i].first->mesh,
                .model = draw_data[i].second->GetMatrix(),
                .color = draw_data[i].first->color,
            };
        }
        return draw_assets;
    }
} // namespace
