#include "SdlWindow.hpp"

#include <iostream>
#include <ostream>

namespace yarep::environment
{
    SdlWindow::SdlWindow()
    {
        m_window = nullptr;
        m_context = {};
    };

    SdlWindow::~SdlWindow() = default;

    void SdlWindow::Setup(WindowConfig config)
    {
        if (SDL_Init(SDL_INIT_VIDEO) != 0)
        {
            throw std::runtime_error("SDL_Init failed: " + std::string(SDL_GetError()));
        }

        uint32_t window_flags = SDL_WINDOW_ALLOW_HIGHDPI;
        switch (config.render_api)
        {
            case Api::OpenGl:
                window_flags |= SDL_WINDOW_OPENGL;
                SetupOpenGl();
                break;
            case Api::Vulkan:
                window_flags |= SDL_WINDOW_VULKAN;
                break;
            case Api::Metal:
                window_flags |= SDL_WINDOW_METAL;
                break;
        }

        m_window = SDL_CreateWindow(config.title.c_str(),
                                    SDL_WINDOWPOS_CENTERED,
                                    SDL_WINDOWPOS_CENTERED,
                                    config.width,
                                    config.height,
                                    window_flags);
        if (m_window == nullptr)
        {
            throw std::runtime_error(SDL_GetError());
        }

        m_context.width = config.width;
        m_context.height = config.height;
        SDL_GL_GetDrawableSize(m_window, &m_context.drawable_width, &m_context.drawable_height);
        switch (config.render_api)
        {
            case Api::OpenGl:
                m_context.open_gl_context = OpenGlContext{
                    .window_handle = m_window,
                    .context = SDL_GL_CreateContext(m_window),
                };
                SDL_GL_MakeCurrent(m_context.open_gl_context.window_handle, m_context.open_gl_context.context);
                SDL_GL_SetSwapInterval(config.vsync ? 1 : 0);
                break;
            case Api::Vulkan:
            case Api::Metal:
                break;
        }
    }

    WindowContext& SdlWindow::GetWindowContext()
    {
        return m_context;
    }

    void SdlWindow::SwapBuffers()
    {
        SDL_GL_SwapWindow(m_window);
    }

    void SdlWindow::Shutdown()
    {
        SDL_DestroyWindow(m_window);
    }

    void SdlWindow::PollEvents(const std::function<void(const SDL_Event&)>& callback)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            callback(event);
        }
    }


    void SdlWindow::SetupOpenGl()
    {
        SDL_GL_ResetAttributes();

        auto ok = [](int rc, const char* name)
        {
            if (rc != 0)
            {
                throw std::runtime_error(
                                         std::string("SDL_GL_SetAttribute failed for") + name + ": " +
                                         std::string(SDL_GetError()));
            }
        };

        ok(SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4), "Major version");
        ok(SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1), "Minor version");
        ok(SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE), "Profile Mask");
        ok(SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG), "Forward Compat");
        ok(SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1), "Double buffering");
        ok(SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24), "Depth size");
        ok(SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8), "Stencil size");
        ok(SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1), "Multisample buffers");
        ok(SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 8), "Multisamples");
    }
}
