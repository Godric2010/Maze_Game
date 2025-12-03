#pragma once
#include <unordered_map>
#include <glm/gtc/matrix_transform.hpp>
#include "ISystem.hpp"

#include "TextController.hpp"
#include "../../interface/include/Input/InputBuffer.hpp"
#include "../../renderer/RenderController.hpp"
#include "../components/ui/RectTransform.hpp"
#include "../components/ui/Text.hpp"

namespace Engine::Core::Systems {
    ECS_SYSTEM(UiSystem, Ui, [ENGINE])

    class UiSystem : public Ecs::IEngineSystem {
    public:
        UiSystem();

        ~UiSystem() override;

        void Initialize() override;

        void Run(float delta_time) override;

    private:
        Text::TextController* m_text_controller = nullptr;
        Renderer::RenderController* m_render_controller = nullptr;
        std::unordered_map<Text::FontHandle, Renderer::TextureHandle> m_font_textures;

        static bool IsMouseOverElement(glm::vec2 mouse_pos, const Components::UI::RectTransform* rect);

        void HandleButtons(const Input::InputBuffer& input) const;

        void HandleTextLabels();

        Renderer::TextureHandle GetOrCreateTextureHandleFromFont(Text::FontHandle font_handle);
    };
} // namespace
