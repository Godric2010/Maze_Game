#pragma once
#include <memory>

#include "AssetHandler.hpp"
#include "IFileManager.hpp"
#include "IRenderController.hpp"
#include "Window.hpp"

namespace yarep::renderer {
    class RenderControllerFactory {
    public:
        static std::unique_ptr<IRenderController> CreateRenderController(
                const environment::WindowContext& window_context, asset_handling::AssetHandler* asset_handler);
    };
} // namespace
