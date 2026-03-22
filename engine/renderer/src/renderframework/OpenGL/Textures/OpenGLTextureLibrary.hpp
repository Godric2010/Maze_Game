#pragma once
#include <unordered_map>

#include "AssetTypes.hpp"
#include "OpenGLTexture.hpp"
#include "../../IGpuTextureLibrary.hpp"

namespace Engine::Renderer::RenderFramework::OpenGl
{
    class OpenGLTextureLibrary : public IGpuTextureLibrary
    {
    public:
        OpenGLTextureLibrary();

        ~OpenGLTextureLibrary() override;

        void AddTexture(const Assets::TextureHandle& texture_handle,
                        const AssetHandling::TextureAsset& texture_asset) override;

        void RemoveTexture(const Assets::TextureHandle& texture_handle) override;

        bool HasTexture(const Assets::TextureHandle& texture_handle) const override;
        
        void ClearTextures() override;

        OpenGLTexture& GetTexture(const Assets::TextureHandle& texture_handle);

    private:
        std::unordered_map<Assets::TextureHandle, OpenGLTexture> m_textures;
    };
} // namespace
