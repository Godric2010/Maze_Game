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

        void Initialize(Ecs::World *world, Ecs::IServiceToEcsProvider *service_locator) override;

        void Run(Ecs::World &world, float delta_time) override;

    private:
        const Renderer::RenderController *m_renderController;

        static Renderer::CameraAsset CreateCameraAsset(const Components::Camera *camera_component,
                                                       const Components::Transform *camera_transform);

        static std::vector<Renderer::DrawAsset> CreateDrawAssets(
            const std::vector<std::pair<Components::Mesh *, Components::Transform *>> &draw_data);
    };
} // namespace
