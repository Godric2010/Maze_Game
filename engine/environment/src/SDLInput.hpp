#pragma once
#include <SDL_events.h>

#include "EnvInput.hpp"
#include "SDLWindow.hpp"

namespace Engine::Environment {
    class SDLInput final : public IEnvInput{
    public:
        explicit SDLInput(SDLWindow& window);

        ~SDLInput() override;

        void PrepareFrame() override;

        void ShowMouseCursor(bool visible) override;

        AppEventsSnapshot *GetAppEventSnapshot() override;

        InputSnapshot *GetInputSnapshot() override;

        void PumpInput() override;

    private:

        std::function<void()> Poll;
        void ProcessInput(const SDL_Event &event);

        AppEventsSnapshot m_appEvents{};
        InputSnapshot m_inputSnapshot;
        glm::vec2 m_lastMousePos{};
    };
} // namespace