//
// Created by sebastian on 05.04.26.
//

#pragma once
#include <array>
#include <string>

namespace yarep::core::settings
{
    # define WINDOW_MODE_LIST \
        X(Windowed) \
        X(Borderless) \
        X(Fullscreen)

    enum class WindowMode
    {
        #define X(name) name,
        WINDOW_MODE_LIST
        #undef X
    };

    constexpr std::array<std::pair<std::string_view, WindowMode>, 3> window_mode_map = {
        {
            #define X(name) {#name, WindowMode::name},
            WINDOW_MODE_LIST
            #undef X
        }
    };

    struct WindowSettings
    {
        int width = 1980; //1280;
        int height = 1280; //720;
        WindowMode mode = WindowMode::Windowed;
        std::string title = "Unknown Game";
    };

    #define RENDER_API_LIST \
        X(OpenGL) \
        X(Metal) \
        X(Vulkan)

    enum class RenderApi
    {
        #define X(name) name,
        RENDER_API_LIST
        #undef X
    };

    constexpr  std::array<std::pair<std::string_view, RenderApi>, 3> render_api_map = {
        {
            #define X(name) {#name, RenderApi::name},
            RENDER_API_LIST
            #undef X
        }
    };

    struct RenderSettings
    {
        RenderApi api = RenderApi::OpenGL;
        bool vsync = false;
    };

    struct EngineSettings
    {
        WindowSettings window;
        RenderSettings render;
    };
}
