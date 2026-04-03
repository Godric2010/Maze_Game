//
// Created by sebastian on 21.03.26.
//

#pragma once
#include "IEngineSystem.hpp"
#include "IRenderController.hpp"
#include "ui/Text.hpp"


namespace Engine::Systems
{
    ECS_SYSTEM(UiTextSystem, Ui, [ENGINE])
    class UiTextSystem : public Ecs::IEngineSystem
    {
        public:
            UiTextSystem();
            ~UiTextSystem() override;

            void Initialize() override;
            void Run(float delta_time) override;

        private:
            std::unordered_map<Text::FontHandle, Assets::TextureHandle> m_font_textures;
            Transform::TransformCache* m_transform_cache = nullptr;
            UI::UiCache* m_ui_cache = nullptr;
            Text::TextController* m_text_controller = nullptr;
            Renderer::IRenderController* m_render_controller = nullptr;
            AssetHandling::AssetHandler* m_asset_handler = nullptr;

            Assets::MaterialHandle RegisterNewUiMaterial() const;

            void RegisterTextElement(Ecs::EntityId entity) const;

            void HandleTextLabels();

            void UpdateTextMesh(Ecs::EntityId entity, UI::UiCache::TextElement text_element,
                                const Components::UI::Text* text) const;

            void RegisterTextureHandleFromFont(Text::FontHandle font_handle);
            void UpdateTextureFromFont(Text::FontHandle font_handle);
    };
}
