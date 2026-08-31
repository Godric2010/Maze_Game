#pragma once
#include <unordered_map>

#include "AssetTypes.hpp"
#include "OpenGLTexture.hpp"
#include "../../../resources/IGpuTextureLibrary.hpp"

namespace yarep::Renderer::RenderFramework::OpenGl
{
    class OpenGLTextureLibrary : public Resources::IGpuTextureLibrary
    {
    public:
        OpenGLTextureLibrary();

        ~OpenGLTextureLibrary() override;

        void AddTexture(const assets::TextureHandle& texture_handle,
                        const AssetHandling::TextureAsset& texture_asset, uint32_t revision) override;

        void RemoveTexture(const assets::TextureHandle& texture_handle) override;

        bool HasTexture(const assets::TextureHandle& texture_handle) const override;
        
        uint32_t GetTextureRevision(const assets::TextureHandle& texture_handle) const override;
        
        void ClearTextures() override;

        OpenGLTexture& GetTexture(const assets::TextureHandle& texture_handle);

    private:
        std::unordered_map<assets::TextureHandle, OpenGLTexture> m_textures;
        std::unordered_map<assets::TextureHandle, uint32_t> m_texture_revisions;
    };
} // namespace
