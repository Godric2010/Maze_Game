//
// Created by sebastian on 22.03.26.
//

#pragma once
#include "AssetTypes.hpp"
#include "Assets/AssetHandleTypes.hpp"

namespace Engine::Renderer::RenderFramework
{
    class IGpuTextureLibrary
    {
    public:
        virtual ~IGpuTextureLibrary() = default;

        virtual void AddTexture(const Assets::TextureHandle& texture_handle,
                                const AssetHandling::TextureAsset& texture_asset) = 0;

        virtual void RemoveTexture(const Assets::TextureHandle& texture_handle) = 0;

        virtual bool HasTexture(const Assets::TextureHandle& texture_handle) const = 0;

        virtual void ClearTextures() = 0;
    };
}
