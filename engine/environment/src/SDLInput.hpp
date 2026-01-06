#pragma once
#include <SDL_events.h>

#include "EnvInput.hpp"
#include "SDLWindow.hpp"

namespace Engine::Environment {
    class SDLInput final : public IEnvInput {
    public:
        explicit SDLInput(SDLWindow& window);

        ~SDLInput() override;

        void PrepareFrame() override;

        void ShowMouseCursor(bool visible) override;

        AppEventsSnapshot* GetAppEventSnapshot() override;

        InputSnapshot* GetInputSnapshot() override;

        void PumpInput() override;

    private:
        std::function<void()> m_poll;

        void ProcessInput(const SDL_Event& event);

        AppEventsSnapshot m_app_events{};
        std::unordered_set<Key> m_keys_down;
        std::unordered_set<Key> m_keys_up;
        std::unordered_set<Key> m_keys_held_pressed;
        std::unordered_set<MouseButton> m_buttons_down;
        std::unordered_set<MouseButton> m_buttons_up;
        std::unordered_set<MouseButton> m_buttons_held_pressed;
        glm::vec2 m_current_mouse_pos;
        glm::vec2 m_current_mouse_delta;
        glm::vec2 m_last_mouse_pos{};
    };
} // namespace
