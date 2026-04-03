#pragma once
#include "../include/Datatypes.hpp"
#include "AssetTypes.hpp"

namespace Engine::Renderer::RenderFramework
{
    /**
     *@class IRenderer
     * Interface to access the renderer and its functions to display an image on the screen
     */
    class IRenderer
    {
        public:
            virtual ~IRenderer() = default;

            /**
             * Initialize the renderer component
             */
            virtual void Initialize() = 0;

            /**
             * Prepare the frame of the renderer
             */
            virtual void PrepareFrame(const CameraAsset& camera_asset) = 0;

            /**
             * Call to render to the screen
             */
            virtual void DrawFrame(std::vector<DrawAsset>& draw_assets) = 0;

            virtual uint32_t GetDrawCalls() = 0;

            /**
             * Disable the renderer and free its resources.
             */
            virtual void Shutdown() = 0;
    };
} // namespace
