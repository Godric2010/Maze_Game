#pragma once
#include "ISystem.hpp"
#include "Mesh.hpp"
#include "../../renderer/RenderController.hpp"
#include "components/Camera.hpp"
#include "components/Transform.hpp"

ECS_SYSTEM(RenderSystem, Render, [])

namespace Engine::Core::Systems {
    class RenderSystem : public Ecs::ISystem {
    public:
        RenderSystem();

        ~RenderSystem() override;

        void SetServices(Ecs::IServiceToEcsProvider *serviceLocator) override;

        void Run(Ecs::World &world, float deltaTime) override;

    private:
        const Renderer::RenderController *m_renderController;

        static Renderer::CameraAsset CreateCameraAsset(const Components::Camera *cameraComponent,
                                                       const Components::Transform *cameraTransform);

        static std::vector<Renderer::DrawAsset> CreateDrawAssets(
            const std::vector<std::pair<Components::Mesh *, Components::Transform *>> &drawData);
    };
} // namespace
