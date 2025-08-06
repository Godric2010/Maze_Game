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
        void PollEvents() override;
        void Shutdown() override;

    };
}