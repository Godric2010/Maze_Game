#pragma once
#include "ISystem.hpp"
#include "Mesh.hpp"
#include "../../renderer/RenderController.hpp"
#include "components/Camera.hpp"
#include "components/Transform.hpp"
#include <ranges>

#include "DebugUIDrawer.hpp"

ECS_SYSTEM(RenderSystem, Render, [ENGINE])

namespace Engine::Core::Systems {
    class RenderSystem : public Ecs::IEngineSystem {
    public:
        RenderSystem();

        ~RenderSystem() override;

        void Initialize() override;

        void Run(float delta_time) override;

    private:
        const Renderer::RenderController *m_render_controller{};
        DebugUiDrawer *m_debug_ui_drawer{};

        static Renderer::CameraAsset CreateCameraAsset(const Components::Camera *camera_component,
                                                       const Components::Transform *camera_transform);

        [[nodiscard]] std::vector<Renderer::MeshDrawAsset> CreateDrawAssets() const;

        [[nodiscard]] std::vector<Renderer::UiDrawAsset> CreateUiDrawAssets() const;
    };
} // namespace
