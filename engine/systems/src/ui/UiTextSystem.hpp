//
// Created by sebastian on 21.03.26.
//

#pragma once
#include "IEngineSystem.hpp"
#include "IRenderController.hpp"
#include "ui/Text.hpp"


namespace yarep::systems {
    ECS_SYSTEM(UiTextSystem, Ui, TAGS(ENGINE), DEPENDENCIES())
    class UiTextSystem : public ecs::IEngineSystem {
    public:
        UiTextSystem();

        ~UiTextSystem() override;

        void Initialize() override;

        void Run(float delta_time) override;

    private:
        std::unordered_map<text::FontHandle, assets::TextureHandle> m_font_textures;
        transform::TransformCache* m_transform_cache = nullptr;
        ui::UiCache* m_ui_cache = nullptr;
        text::TextController* m_text_controller = nullptr;
        renderer::IRenderController* m_render_controller = nullptr;
        asset_handling::AssetHandler* m_asset_handler = nullptr;

        assets::MaterialHandle RegisterNewUiMaterial() const;

        void RegisterTextElement(ecs::EntityId entity) const;

        void HandleTextLabels();

        void UpdateTextMesh(ecs::EntityId entity, ui::UiCache::TextElement text_element,
                            const components::ui::Text* text) const;

        void RegisterTextureHandleFromFont(text::FontHandle font_handle);

        void UpdateTextureFromFont(text::FontHandle font_handle);
    };
}
