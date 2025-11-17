#include "OpenGLTextureManager.hpp"

#include <ranges>

namespace Engine::Renderer::RenderFramework::OpenGl {
    OpenGLTextureManager::OpenGLTextureManager() = default;

    OpenGLTextureManager::~OpenGLTextureManager() = default;

    TextureHandle OpenGLTextureManager::AddTexture(const TextureAsset &texture_asset) {
        TextureHandle texture_handle = m_texture_handle;
        ++m_texture_handle;

        OpenGLTexture texture{};
        texture.width = texture_asset.width;
        texture.height = texture_asset.height;

        glGenTextures(1, &texture.texture_id);

        glBindTexture(GL_TEXTURE_2D, texture.texture_id);
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);

        glTexImage2D(GL_TEXTURE_2D,
                     0,
                     GL_RED,
                     static_cast<GLint>(texture.width),
                     static_cast<GLint>(texture.height),
                     0,
                     GL_RED,
                     GL_UNSIGNED_BYTE,
                     texture_asset.pixels.data());
        m_textures.emplace(texture_handle, texture);
        return texture_handle;
    }

    OpenGLTexture &OpenGLTextureManager::GetTexture(const TextureHandle &texture_handle) {
        const auto it = m_textures.find(texture_handle);
        if (it != m_textures.end()) {
            return it->second;
        }
        throw std::runtime_error("No such texture handle: " + std::to_string(texture_handle));
    }

    void OpenGLTextureManager::RemoveTexture(const TextureHandle &texture_handle) {
        m_textures.erase(texture_handle);
    }

    void OpenGLTextureManager::Clear() {
        for (auto &val: m_textures | std::views::values) {
            glDeleteTextures(1, &val.texture_id);
        }

        m_textures.clear();
    }
} // namespace
