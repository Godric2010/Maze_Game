//
// Created by Sebastian Borsch on 14.12.25.
//

#pragma once
#include "Datatypes.hpp"
#include <memory>
#include "Renderer/IRenderer.hpp"
#include "AssetTypes.hpp"

namespace Engine::Renderer
{
    class IRenderController : public IRenderer
    {
    public:
        ~IRenderController() override = default;

        virtual void RegisterMesh(const AssetHandling::MeshAsset& mesh, const Assets::MeshHandle& handle) const =0;

        virtual void UnregisterMesh(const Assets::MeshHandle& handle) const = 0;

        virtual void RegisterTexture(const AssetHandling::TextureAsset& texture,
                                     const Assets::TextureHandle& handle) const = 0;

        virtual void UnregisterTexture(const Assets::TextureHandle& handle) const = 0;

        virtual void BeginFrame(const CameraAsset& camera_asset) const = 0;

        virtual void SubmitFrame(DrawAssets& draw_assets) const = 0;

        virtual void SubmitDebugInfos(const std::vector<UiDrawAsset>& debug_draw_assets) = 0;

        virtual Assets::MeshHandle GetUIMeshHandle() const = 0;

        [[nodiscard]] virtual uint32_t GetDrawCalls() const = 0;
    };
}
