#pragma once
#include <memory>
#include <unordered_map>

#include "AssetTypes.hpp"
#include "Datatypes.hpp"
#include "OpenGLTexture.hpp"

namespace Engine::Renderer::RenderFramework::OpenGl {
    class OpenGLTextureManager {
    public:
        OpenGLTextureManager();

        ~OpenGLTextureManager();

        void AddTexture(const AssetHandling::TextureAsset &texture_asset, Assets::TextureHandle texture_handle);

        OpenGLTexture &GetTexture(const Assets::TextureHandle &texture_handle);

        void RemoveTexture(const Assets::TextureHandle &texture_handle);

        void Clear();

    private:
        std::unordered_map<Assets::TextureHandle, OpenGLTexture> m_textures;
    };
} // namespace
