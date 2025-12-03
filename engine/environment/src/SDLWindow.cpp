#include "SDLWindow.hpp"

#include <iostream>
#include <ostream>

namespace Engine::Environment {
    SDLWindow::SDLWindow() {
        m_window = nullptr;
        m_context = {};
    };

    SDLWindow::~SDLWindow() = default;

    void SDLWindow::Setup(WindowConfig config) {
        if (SDL_Init(SDL_INIT_VIDEO) != 0) {
            throw std::runtime_error("SDL_Init failed: " + std::string(SDL_GetError()));
        }

        uint32_t windowFlags = SDL_WINDOW_ALLOW_HIGHDPI;
        switch (config.renderApi) {
            case API::OpenGL:
                windowFlags |= SDL_WINDOW_OPENGL;
                SetupOpenGL();
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
        SDL_GL_GetDrawableSize(m_window, &m_context.drawableWidth, &m_context.drawableHeight);
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

    WindowContext &SDLWindow::GetWindowContext() {
        return m_context;
    }

    bool SDLWindow::PollEvents_old() {
        SDL_Event event;
        SDL_PollEvent(&event);
        switch (event.type) {
            case SDL_QUIT:
                return false;
            default:
                return true;
        }
    }

    void SDLWindow::SwapBuffers() {
        SDL_GL_SwapWindow(m_window);
    }

    void SDLWindow::Shutdown() {
        SDL_DestroyWindow(m_window);
    }

    void SDLWindow::PollEvents(const std::function<void(const SDL_Event &)> &callback) {
        SDL_Event event;
        SDL_PollEvent(&event);
        callback(event);
    }


    void SDLWindow::SetupOpenGL() {
        SDL_GL_ResetAttributes();

        auto ok = [](int rc, const char *name) {
            if (rc != 0) {
                throw std::runtime_error(
                    std::string("SDL_GL_SetAttribute failed for") + name + ": " + std::string(SDL_GetError()));
            }
        };

        ok(SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4), "Major version");
        ok(SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1), "Minor version");
        ok(SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE), "Profile Mask");
        ok(SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG), "Forward Compat");
        ok(SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1), "Double buffering");
        ok(SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24), "Depth size");
        ok(SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8), "Stencil size");
    }
}
