#pragma once
#include "IEngineSystem.hpp"
#include "IRenderController.hpp"

namespace yarep::systems {
    ECS_SYSTEM(UiButtonSystem, Ui, TAGS(ENGINE), DEPENDENCIES())
    class UiButtonSystem : public ecs::IEngineSystem {
    public:
        UiButtonSystem();

        ~UiButtonSystem() override;

        void Initialize() override;

        void Run(float delta_time) override;

    private:
        transform::TransformCache* m_transform_cache = nullptr;
        ui::UiCache* m_ui_cache = nullptr;
        renderer::IRenderController* m_render_controller = nullptr;
        asset_handling::AssetHandler* m_asset_handler = nullptr;

        void RegisterButtonElement(ecs::EntityId entity, glm::vec4 color) const;

        assets::MaterialHandle RegisterNewUiMaterial() const;

        bool IsMouseOverElement(glm::vec2 mouse_pos, const ecs::EntityId& rect_entity) const;

        void HandleButtons(const input::InputBuffer& input) const;
    };
}
