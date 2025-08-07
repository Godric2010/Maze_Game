#include "SDLWindow.hpp"

#include <iostream>
#include <ostream>

namespace Engine::Window {
    SDLWindow::SDLWindow() {
        m_window = nullptr;
        m_context = {};
    };

    SDLWindow::~SDLWindow() = default;

    void SDLWindow::Setup(WindowConfig config) {
        uint32_t windowFlags = 0;
        switch (config.renderApi) {
            case API::OpenGL:
                windowFlags |= SDL_WINDOW_OPENGL;
                SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION,4);
                SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION,1);
                SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
                SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
                SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
                SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
                break;
            case API::Vulkan:
                windowFlags |= SDL_WINDOW_VULKAN;
                break;
            case API::Metal:
                windowFlags |= SDL_WINDOW_METAL;
                break;
        }

        m_window = SDL_CreateWindow(config.title.c_str(),
                                    SDL_WINDOWPOS_CENTERED,
                                    SDL_WINDOWPOS_CENTERED,
                                    config.width,
                                    config.height,
                                    windowFlags);
        if (m_window == nullptr) {
            throw std::runtime_error(SDL_GetError());
        }

        m_context.width = config.width;
        m_context.height = config.height;
        switch (config.renderApi) {
            case API::OpenGL:
                m_context.openGLContext = OpenGLContext{
                    .windowHandle = m_window,
                    .context = SDL_GL_CreateContext(m_window),
                };
                SDL_GL_SetSwapInterval(1);
                break;
            case API::Vulkan:
            case API::Metal:
                break;
        }
    }

    WindowContext SDLWindow::GetWindowContext() {
        return m_context;
    }

    bool SDLWindow::PollEvents() {
        SDL_Event event;
        SDL_PollEvent(&event);
        switch (event.type) {
            case SDL_QUIT:
                return false;
            default:
                return true;
        }
    }

    void SDLWindow::Shutdown() {
        SDL_DestroyWindow(m_window);
    }
}
