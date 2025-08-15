#pragma once
#include <SDL_events.h>

#include "Input.hpp"
#include "SDLWindow.hpp"

namespace Engine::Environment {
    class SDLInput final : public IInput{
    public:
        explicit SDLInput(SDLWindow& window);

        ~SDLInput() override;

        void PrepareFrame() override;

        InputSnapshot GetInputSnapshot() override;

        void PumpInput() override;

    private:

        std::function<void()> Poll;
        void ProcessInput(const SDL_Event &event);

        InputSnapshot m_inputSnapshot;
        glm::vec2 m_lastMousePos{};
    };
} // namespace