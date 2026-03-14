#pragma once
#include <IEngineSystem.hpp>
#include <unordered_map>

#include <Input/InputBuffer.hpp>
#include <ui/RectTransform.hpp>

#include <IRenderController.hpp>
#include <TextController.hpp>
#include "../CacheManager.hpp"
#include "ui/Text.hpp"

namespace Engine::Systems {
    ECS_SYSTEM(UiSystem, Ui, [ENGINE])

    class UiSystem : public Ecs::IEngineSystem {
    public:
        UiSystem();

        ~UiSystem() override;

        void Initialize() override;

        void Run(float delta_time) override;

    private:
        Transform::TransformCache* m_transform_cache = nullptr;
        UI::UiCache* m_ui_cache = nullptr;
        Text::TextController* m_text_controller = nullptr;
        Renderer::IRenderController* m_render_controller = nullptr;
        AssetHandling::AssetHandler* m_asset_handler = nullptr;
        std::unordered_map<Text::FontHandle, Assets::TextureHandle> m_font_textures;

        void RegisterColorElement(Ecs::EntityId entity, glm::vec4 color);
        
        void RegisterTextElement(Ecs::EntityId entity) const;
        
        Assets::MaterialHandle RegisterNewUiMaterial() const;
        
        bool IsMouseOverElement(glm::vec2 mouse_pos, const Ecs::EntityId& rect_entity) const;

        void HandleButtons(const Input::InputBuffer& input) const;

        void HandleTextLabels();
        
        void BuildNewTextMesh(Ecs::EntityId entity, UI::UiCache::TextElement text_element, const Components::UI::Text* text) const;

        Assets::TextureHandle GetOrCreateTextureHandleFromFont(Text::FontHandle font_handle);
    };
} // namespace
