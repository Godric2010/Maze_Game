#pragma once
#include "ISystem.hpp"
#include "Mesh.hpp"
#include "../../renderer/RenderController.hpp"
#include "components/Camera.hpp"
#include "components/Transform.hpp"

ECS_SYSTEM(RenderSystem, Render, [ENGINE])

namespace Engine::Core::Systems {
    class RenderSystem : public Ecs::IEngineSystem {
    public:
        RenderSystem();

        ~RenderSystem() override;

        void Initialize() override;

        void Run(float delta_time) override;

    private:
        const Renderer::RenderController *m_renderController;

        static Renderer::CameraAsset CreateCameraAsset(const Components::Camera *camera_component,
                                                       const Components::Transform *camera_transform);

        static std::vector<Renderer::MeshDrawAsset> CreateDrawAssets(
            const std::vector<std::pair<Components::Mesh *, Components::Transform *> > &draw_data);
    };
} // namespace
