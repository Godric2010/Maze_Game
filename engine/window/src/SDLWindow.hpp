//
// Created by Sebastian Borsch on 04.08.25.
//

#pragma once
#include "../include/Window.hpp"

namespace Engine::Window {
    class SDLWindow final : public IWindow{

    public:
        SDLWindow();
        ~SDLWindow() override;

        void Setup(WindowConfig config) override;
        WindowContext GetWindowContext() override;

        bool PollEvents() override;
        void SwapBuffers() override;
        void Shutdown() override;

    private:
        SDL_Window* m_window;
        WindowContext m_context;

    };
}