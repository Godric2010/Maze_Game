//
// Created by Sebastian Borsch on 04.08.25.
//

#pragma once
#define SDL_MAIN_HANDLED
#include <SDL.h>
#include <string>

namespace yarep::environment {
    /**
     * @enum Api
     * @brief Represents various graphics APIs that can be used in the application.
     */
    enum class Api {
        OpenGl,
        Vulkan, // Not implemented, only here for reference
        Metal, // Not implemented, only here for reference
    };

    /**
     * @enum WindowMode
     * @brief Represents the various modes, the window can be created in
     */
    enum class WindowMode {
        Window,
        Fullscreen, // Not implemented, only here for reference
        Borderless, // Not implemented, only here for reference
    };

    /**
     * @struct WindowConfig
     * @brief Configuration struct for creating a window
     */
    struct WindowConfig {
        int width;
        int height;
        std::string title;
        Api render_api;
        WindowMode window_mode;
        bool vsync;
    };

    /**
     *@struct OpenGlContext
     * The OpenGL context contains all necessary data to initialize
     * an OpenGL Renderer with this window
     */
    struct OpenGlContext {
        SDL_Window *window_handle;
        SDL_GLContext context;
    };

    /**
     *@struct WindowContext
     * The window context contains all data to initialize a renderer in this window
     */
    struct WindowContext {
        int width{};
        int height{};
        int drawable_width{};
        int drawable_height{};
        Api render_api{};
        OpenGlContext open_gl_context{};
    };


    /**
     * @class IWindow
     * @brief Interface representing a generic window for the application.
     *
     * The IWindow interface provides the necessary methods for setting up,
     * managing events, and shutting down a window. Concrete implementations
     * of this interface are responsible for specific windowing details.
     */
    class IWindow {
    public:
        virtual ~IWindow() = default;

        /**
         * Set up a window based on the given configuration
         * @param config The configuration details of this window
         */
        virtual void Setup(WindowConfig config) = 0;

        /**
         * Get the context of the window. Depending on the type of API in use, the
         * context might vary.
         * @return The context data of this window, necessary to connect a renderer to it
         */
        virtual WindowContext &GetWindowContext() = 0;

        /**
         * Swap the window buffers
         */
        virtual void SwapBuffers() = 0;

        /**
         * Destroy the window and all its content. Note: Destroy all dependencies
         * in this window first to avoid errors.
         */
        virtual void Shutdown() = 0;
    };
}
