#include "../include/RenderControllerFactory.hpp"

#include "RenderController.hpp"

namespace yarep::renderer {
    std::unique_ptr<IRenderController> RenderControllerFactory::CreateRenderController(
            const environment::WindowContext& window_context, asset_handling::
            AssetHandler* asset_handler) {
        return std::make_unique<RenderController>(window_context,  asset_handler);
    }
} // namespace
