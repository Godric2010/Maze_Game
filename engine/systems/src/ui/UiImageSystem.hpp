#pragma once
#include <IEngineSystem.hpp>

#include <ui/RectTransform.hpp>

#include <IRenderController.hpp>
#include <TextController.hpp>
#include "../CacheManager.hpp"

namespace yarep::systems {
    ECS_SYSTEM(UiImageSystem, Ui, TAGS(ENGINE), DEPENDENCIES())

    class UiImageSystem : public ecs::IEngineSystem {
    public:
        UiImageSystem();

        ~UiImageSystem() override;

        void Initialize() override;

        void Run(float delta_time) override;

    private:
        transform::TransformCache* m_transform_cache = nullptr;
        ui::UiCache* m_ui_cache = nullptr;
        renderer::IRenderController* m_render_controller = nullptr;
        asset_handling::AssetHandler* m_asset_handler = nullptr;

        void RegisterImageElement(ecs::EntityId entity, glm::vec4 color) const;

        assets::MaterialHandle RegisterNewUiMaterial() const;
    };
} // namespace
