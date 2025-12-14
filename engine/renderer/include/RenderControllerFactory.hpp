#pragma once
#include <memory>

#include "IRenderController.hpp"
#include "Window.hpp"

namespace Engine::Renderer {
    class RenderControllerFactory {
    public:
        static std::unique_ptr<IRenderController> CreateRenderController(
                const Environment::WindowContext& window_context);
    };
} // namespace
