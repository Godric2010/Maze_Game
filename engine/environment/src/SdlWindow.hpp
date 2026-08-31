//
// Created by Sebastian Borsch on 04.08.25.
//

#pragma once
#include <functional>

#include "../include/Window.hpp"

namespace yarep::environment {
    class SdlWindow final : public IWindow{

    public:
        SdlWindow();
        ~SdlWindow() override;

        void Setup(WindowConfig config) override;

        WindowContext &GetWindowContext() override;

        void SwapBuffers() override;
        void Shutdown() override;

        void PollEvents(const std::function<void(const SDL_Event &)> &callback);


    private:

        static void SetupOpenGl();

        SDL_Window* m_window;
        WindowContext m_context;
    };
}