#pragma once
#include <unordered_map>

#include "AssetTypes.hpp"
#include "OpenGlTexture.hpp"
#include "../../../resources/IGpuTextureLibrary.hpp"

namespace yarep::renderer::render_framework::open_gl
{
    class OpenGlTextureLibrary : public resources::IGpuTextureLibrary
    {
    public:
        OpenGlTextureLibrary();

        ~OpenGlTextureLibrary() override;

        void AddTexture(const assets::TextureHandle& texture_handle,
                        const asset_handling::TextureAsset& texture_asset, uint32_t revision) override;

        void RemoveTexture(const assets::TextureHandle& texture_handle) override;

        bool HasTexture(const assets::TextureHandle& texture_handle) const override;
        
        uint32_t GetTextureRevision(const assets::TextureHandle& texture_handle) const override;
        
        void ClearTextures() override;

        OpenGlTexture& GetTexture(const assets::TextureHandle& texture_handle);

    private:
        std::unordered_map<assets::TextureHandle, OpenGlTexture> m_textures;
        std::unordered_map<assets::TextureHandle, uint32_t> m_texture_revisions;
    };
} // namespace
