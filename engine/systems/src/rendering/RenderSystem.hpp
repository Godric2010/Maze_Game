#pragma once
#include <IRenderController.hpp>
#include <IEngineSystem.hpp>
#include <Transform.hpp>

ECS_SYSTEM(RenderSystem, Render, [ENGINE])

namespace Engine::Systems {
    class RenderSystem : public Ecs::IEngineSystem {
    public:
        RenderSystem();

        ~RenderSystem() override;

        void Initialize() override;

        void Run(float delta_time) override;

    private:
        const Renderer::IRenderController* m_render_controller{};

        Renderer::CameraAsset CreateCameraAsset(const Ecs::EntityId& camera_entity,
                                                const Components::Transform* camera_transform) const;

        [[nodiscard]] std::vector<Renderer::MeshDrawAsset> CreateDrawAssets() const;

        [[nodiscard]] std::vector<Renderer::UiDrawAsset> CreateUiDrawAssets() const;
    };
} // namespace
