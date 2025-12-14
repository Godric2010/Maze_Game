#include "../include/RenderControllerFactory.hpp"

#include "RenderController.hpp"

namespace Engine::Renderer {
    std::unique_ptr<IRenderController> RenderControllerFactory::CreateRenderController(
            const Environment::WindowContext& window_context) {
        return std::make_unique<RenderController>(window_context);
    }
} // namespace
