#include "RenderSystem.hpp"

#include "Mesh.hpp"
#include "components/Camera.hpp"
#include "components/Transform.hpp"

namespace Engine::Core::Systems {
    RenderSystem::RenderSystem() = default;

    RenderSystem::~RenderSystem() = default;

    void RenderSystem::SetServices(Ecs::IServiceToEcsProvider *serviceLocator) {
        const Renderer::RenderController *renderController = serviceLocator->GetService<Renderer::RenderController>();
        m_renderController = renderController;
    }

    void RenderSystem::Run(Ecs::World &world, float deltaTime) {
        const auto [camera, cameraEntity] = world.GetComponentsOfType<Components::Camera>()[0];
        const auto cameraTransform = world.GetComponent<Components::Transform>(cameraEntity);
        const auto cameraAsset = CreateCameraAsset(camera, cameraTransform);

        const auto meshComponents = world.GetComponentsOfType<Components::Mesh>();
        auto drawData = std::vector<std::pair<Components::Mesh *, Components::Transform *> >(meshComponents.size());
        for (size_t i = 0; i < meshComponents.size(); ++i) {
            const auto [mesh, meshEntity] = meshComponents[i];
            auto meshTransform = world.GetComponent<Components::Transform>(meshEntity);
            const auto meshData = std::make_pair(mesh, meshTransform);
            drawData[i] = meshData;
        }
        const std::vector<Renderer::DrawAsset> drawAssets = CreateDrawAssets(drawData);

        m_renderController->BeginFrame(cameraAsset);
        m_renderController->SubmitFrame(drawAssets);
    }

    Renderer::CameraAsset RenderSystem::CreateCameraAsset(const Components::Camera *cameraComponent,
                                                          const Components::Transform *cameraTransform) {
        const Renderer::CameraAsset cameraAsset{
            .view = cameraComponent->view,
            .projection = cameraComponent->projection,
            .cameraPosition = glm::vec4(cameraTransform->Position, 1.0f)

        };
        return cameraAsset;
    }

    std::vector<Renderer::DrawAsset> RenderSystem::CreateDrawAssets(
        const std::vector<std::pair<Components::Mesh *, Components::Transform *> > &drawData) {
        auto drawAssets = std::vector<Renderer::DrawAsset>(drawData.size());
        for (size_t i = 0; i < drawData.size(); ++i) {
            drawAssets[i] = Renderer::DrawAsset{
                .mesh = drawData[i].first->mesh,
                .model = drawData[i].second->Matrix,
                .color = drawData[i].first->color,
            };
        }
        return drawAssets;
    }
} // namespace
