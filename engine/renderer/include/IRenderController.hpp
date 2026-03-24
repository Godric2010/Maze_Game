//
// Created by Sebastian Borsch on 14.12.25.
//

#pragma once
#include "Datatypes.hpp"
#include <memory>
#include "AssetTypes.hpp"

namespace Engine::Renderer
{
    class IRenderController 
    {
    public:
        virtual ~IRenderController() = default;

        virtual void RegisterTexture(const AssetHandling::TextureAsset& texture,
                                     const Assets::TextureHandle& handle) const = 0;

        virtual void UnregisterTexture(const Assets::TextureHandle& handle) const = 0;

        virtual void RegisterMaterial(const Assets::MaterialHandle& material_handle) const = 0;

        virtual void UnregisterMaterial(const Assets::MaterialHandle& material_handle) const = 0;

        virtual void SubmitDebugInfos(const std::vector<DrawAsset>& debug_draw_assets) = 0;

        virtual void RenderFrame(const CameraAsset& camera_asset, std::vector<DrawAsset> draw_assets) const = 0;

        virtual Assets::MeshHandle GetUIMeshHandle() const = 0;

        [[nodiscard]] virtual uint32_t GetDrawCalls() const = 0;
    };
}
