#pragma once
#include <memory>

#include "IFileReader.hpp"
#include "IRenderController.hpp"
#include "Window.hpp"

namespace Engine::Renderer {
    class RenderControllerFactory {
    public:
        static std::unique_ptr<IRenderController> CreateRenderController(
                const Environment::WindowContext& window_context, Environment::Files::IFileReader* file_reader);
    };
} // namespace
