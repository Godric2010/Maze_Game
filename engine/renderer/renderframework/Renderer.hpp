#pragma once
#include "../Datatypes.hpp"

namespace Engine::Renderer::RenderFramework {
    /**
     *@class IRenderer
     * Interface to access the renderer and its functions to display an image on the screen
     */
    class IRenderer {
    public:
        virtual ~IRenderer() = default;

        /**
         * Initialize the renderer component
         */
        virtual void Initialize() = 0;

        /**
         * Prepare the frame of the renderer
         */
        virtual void PrepareFrame(const CameraAsset& cameraAsset) = 0;

        /**
         * Call to render to the screen
         */
        virtual void DrawFrame(const std::vector<DrawAsset> &drawAssets) = 0;

       /**
        * Add a mesh to the renderer to display it
        */
        virtual MeshHandle AddMesh(const MeshAsset &mesh) = 0;

       /**
        * Remove a mesh from the renderer to free its resources.
        */
        virtual void RemoveMesh(const MeshHandle &meshHandle) = 0;

        /**
         * Disable the renderer and free its resources.
         */
        virtual void Shutdown() = 0;
    };
} // namespace
