#pragma once
#include "IEngineSystem.hpp"
#include "IRenderController.hpp"

namespace Engine::Systems {
    ECS_SYSTEM(UiButtonSystem, Ui, TAGS(ENGINE), DEPENDENCIES())
    class UiButtonSystem : public Ecs::IEngineSystem {
    public:
        UiButtonSystem();

        ~UiButtonSystem() override;

        void Initialize() override;

        void Run(float delta_time) override;

    private:
        Transform::TransformCache* m_transform_cache = nullptr;
        UI::UiCache* m_ui_cache = nullptr;
        Renderer::IRenderController* m_render_controller = nullptr;
        AssetHandling::AssetHandler* m_asset_handler = nullptr;

        void RegisterButtonElement(Ecs::EntityId entity, glm::vec4 color) const;

        Assets::MaterialHandle RegisterNewUiMaterial() const;

        bool IsMouseOverElement(glm::vec2 mouse_pos, const Ecs::EntityId& rect_entity) const;

        void HandleButtons(const Input::InputBuffer& input) const;
    };
}
