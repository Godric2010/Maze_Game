#pragma once
#include "../include/Datatypes.hpp"
#include "AssetTypes.hpp"
#include <memory>

#include "materials/MaterialLibrary.hpp"

namespace Engine::Renderer::RenderFramework
{
    /**
     *@class IRenderer
     * Interface to access the renderer and its functions to display an image on the screen
     */
    class IRenderer
    {
    public:
        IRenderer(std::unique_ptr<Materials::MaterialLibrary> material_library)
        {
            m_material_library = std::move(material_library);
        }

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
        virtual void DrawFrame(DrawAssets& draw_assets) = 0;

        /**
         * Add a mesh to the renderer to display it
         */
        virtual void AddMesh(const AssetHandling::MeshAsset& mesh, const Assets::MeshHandle& handle) = 0;

        /**
         * Remove a mesh from the renderer to free its resources.
         */
        virtual void RemoveMesh(const Assets::MeshHandle& mesh_handle) = 0;

        /*
         * Add a texture to the renderer to use it for objects.
         */
        virtual void AddTexture(const AssetHandling::TextureAsset& texture, const Assets::TextureHandle& handle) = 0;

        /*
         * Remove a texture from the renderer to free its resources.
         */
        virtual void RemoveTexture(const Assets::TextureHandle& texture_handle) = 0;

        virtual uint32_t GetDrawCalls() = 0;

        /**
         * Disable the renderer and free its resources.
         */
        virtual void Shutdown() = 0;

        [[nodiscard]] Materials::MaterialLibrary* GetMaterialLibrary() const
        {
            return m_material_library.get();
        }

    protected:
        std::unique_ptr<Materials::MaterialLibrary> m_material_library;
    };
} // namespace
