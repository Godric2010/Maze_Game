//
// Created by Sebastian Borsch on 04.08.25.
//

#pragma once
#include <functional>

#include "../include/Window.hpp"

namespace Engine::Environment {
    class SDLWindow final : public IWindow{

    public:
        SDLWindow();
        ~SDLWindow() override;

        void Setup(WindowConfig config) override;
        WindowContext GetWindowContext() override;

        bool PollEvents_old() override;
        void SwapBuffers() override;
        void Shutdown() override;

        void PollEvents(const std::function<void(const SDL_Event &)> &callback);


    private:

        static void SetupOpenGL();

        SDL_Window* m_window;
        WindowContext m_context;
    };
}