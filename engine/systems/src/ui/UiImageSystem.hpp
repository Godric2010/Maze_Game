#pragma once
#include <IEngineSystem.hpp>

#include <ui/RectTransform.hpp>

#include <IRenderController.hpp>
#include <TextController.hpp>
#include "../CacheManager.hpp"

namespace Engine::Systems {
    ECS_SYSTEM(UiImageSystem, Ui, TAGS(ENGINE), DEPENDENCIES())

    class UiImageSystem : public Ecs::IEngineSystem {
    public:
        UiImageSystem();

        ~UiImageSystem() override;

        void Initialize() override;

        void Run(float delta_time) override;

    private:
        Transform::TransformCache* m_transform_cache = nullptr;
        UI::UiCache* m_ui_cache = nullptr;
        Renderer::IRenderController* m_render_controller = nullptr;
        AssetHandling::AssetHandler* m_asset_handler = nullptr;

        void RegisterImageElement(Ecs::EntityId entity, glm::vec4 color) const;

        Assets::MaterialHandle RegisterNewUiMaterial() const;
    };
} // namespace
