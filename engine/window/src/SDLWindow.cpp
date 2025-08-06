#include "SDLWindow.hpp"

#include <iostream>
#include <ostream>

namespace Engine::Window {
    SDLWindow::SDLWindow() = default;

    SDLWindow::~SDLWindow() = default;

    void SDLWindow::Setup(WindowConfig config) {
        std::cout << "SDLWindow::Setup" << std::endl;
    }

    WindowContext SDLWindow::GetWindowContext() {
        std::cout << "SDLWindow::GetWindowContext" << std::endl;
        return WindowContext{};
    }

    void SDLWindow::PollEvents() {
        std::cout << "SDLWindow::PollEvents" << std::endl;
    }

    void SDLWindow::Shutdown() {
        std::cout << "SDLWindow::Shutdown" << std::endl;
    }
}
