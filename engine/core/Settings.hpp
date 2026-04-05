//
// Created by sebastian on 05.04.26.
//

#pragma once

namespace Engine::Core
{
    # define WindowModeList \
        X(Windowed) \
        X(Borderless) \
        X(Fullscreen)

    enum class WindowMode
    {
        #define X(name) name,
        WindowModeList
        #undef X
    };

    struct WindowSettings
    {
        int width = 1980; //1280;
        int height = 1280; //720;
        WindowMode mode = WindowMode::Windowed;
        std::string title = "Unknown Game";
    };

    #define RenderApiList \
        X(OpenGL) \
        X(Metal) \
        X(Vulkan)

    enum class RenderApi
    {
        #define X(name) name,
        RenderApiList
        #undef X
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
