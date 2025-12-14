#pragma once
#include <unordered_map>

#include "Datatypes.hpp"
#include "OpenGLTexture.hpp"

namespace Engine::Renderer::RenderFramework::OpenGl {
    class OpenGLTextureManager {
    public:
        OpenGLTextureManager();

        ~OpenGLTextureManager();

        TextureHandle AddTexture(const TextureAsset &texture_asset);

        OpenGLTexture &GetTexture(const TextureHandle &texture_handle);

        void RemoveTexture(const TextureHandle &texture_handle);

        void Clear();

    private:
        TextureHandle m_texture_handle = 1;
        std::unordered_map<TextureHandle, OpenGLTexture> m_textures;
    };
} // namespace
