//
// Created by sebastian on 22.03.26.
//

#pragma once
#include "AssetTypes.hpp"
#include "Assets/AssetHandleTypes.hpp"

namespace Engine::Renderer::Resources
{
    class IGpuTextureLibrary
    {
    public:
        virtual ~IGpuTextureLibrary() = default;

        virtual void AddTexture(const Assets::TextureHandle& texture_handle,
                                const AssetHandling::TextureAsset& texture_asset, uint32_t revision) = 0;

        virtual void RemoveTexture(const Assets::TextureHandle& texture_handle) = 0;

        virtual bool HasTexture(const Assets::TextureHandle& texture_handle) const = 0;

        virtual uint32_t GetTextureRevision(const Assets::TextureHandle& texture_handle) const = 0;

        virtual void ClearTextures() = 0;
    };
}
