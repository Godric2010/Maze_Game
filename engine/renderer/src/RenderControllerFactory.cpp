#include "../include/RenderControllerFactory.hpp"

#include "RenderController.hpp"

namespace Engine::Renderer {
    std::unique_ptr<IRenderController> RenderControllerFactory::CreateRenderController(
            const Environment::WindowContext& window_context, AssetHandling::
            AssetHandler* asset_handler) {
        return std::make_unique<RenderController>(window_context,  asset_handler);
    }
} // namespace
