#pragma once
#include <ISystem.hpp>
#include <unordered_map>

#include <Input/InputBuffer.hpp>
#include <ui/RectTransform.hpp>

#include <IRenderController.hpp>
#include <TextController.hpp>
#include "../CacheManager.hpp"

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
        std::unordered_map<Text::FontHandle, Renderer::TextureHandle> m_font_textures;

        bool IsMouseOverElement(glm::vec2 mouse_pos, const Ecs::EntityId& rect_entity) const;

        void HandleButtons(const Input::InputBuffer& input) const;

        void HandleTextLabels();

        Renderer::TextureHandle GetOrCreateTextureHandleFromFont(Text::FontHandle font_handle);
    };
} // namespace
