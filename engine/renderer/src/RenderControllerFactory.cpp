#include "../include/RenderControllerFactory.hpp"

#include "RenderController.hpp"

namespace Engine::Renderer {
    std::unique_ptr<IRenderController> RenderControllerFactory::CreateRenderController(
            const Environment::WindowContext& window_context, Environment::Files::IFileReader* file_reader) {
        return std::make_unique<RenderController>(window_context, file_reader);
    }
} // namespace
