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
        virtual void PrepareFrame(const CameraAsset &camera_asset) = 0;

        /**
         * Call to render to the screen
         */
        virtual void DrawFrame(DrawAssets &draw_assets) = 0;

        /**
         * Add a mesh to the renderer to display it
         */
        virtual MeshHandle AddMesh(const MeshAsset &mesh) = 0;

        /**
         * Remove a mesh from the renderer to free its resources.
         */
        virtual void RemoveMesh(const MeshHandle &mesh_handle) = 0;

        /*
         * Add a texture to the renderer to use it for objects.
         */
        virtual TextureHandle AddTexture(const TextureAsset &texture) = 0;

        /*
         * Remove a texture from the renderer to free its resources.
         */
        virtual void RemoveTexture(const TextureHandle &texture_handle) = 0;

        virtual uint32_t GetDrawCalls() = 0;

        /**
         * Disable the renderer and free its resources.
         */
        virtual void Shutdown() = 0;
    };
} // namespace
