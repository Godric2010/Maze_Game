#pragma once
#include <unordered_map>

#include "AssetTypes.hpp"
#include "OpenGLTexture.hpp"
#include "../../../resources/IGpuTextureLibrary.hpp"

namespace Engine::Renderer::RenderFramework::OpenGl
{
    class OpenGLTextureLibrary : public Resources::IGpuTextureLibrary
    {
    public:
        OpenGLTextureLibrary();

        ~OpenGLTextureLibrary() override;

        void AddTexture(const Assets::TextureHandle& texture_handle,
                        const AssetHandling::TextureAsset& texture_asset, uint32_t revision) override;

        void RemoveTexture(const Assets::TextureHandle& texture_handle) override;

        bool HasTexture(const Assets::TextureHandle& texture_handle) const override;
        
        uint32_t GetTextureRevision(const Assets::TextureHandle& texture_handle) const override;
        
        void ClearTextures() override;

        OpenGLTexture& GetTexture(const Assets::TextureHandle& texture_handle);

    private:
        std::unordered_map<Assets::TextureHandle, OpenGLTexture> m_textures;
        std::unordered_map<Assets::TextureHandle, uint32_t> m_texture_revisions;
    };
} // namespace
