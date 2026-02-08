//
// Created by Sebastian Borsch on 16.11.25.
//

#pragma once
#include <glad/glad.h>
#include <stdexcept>
#include <string>

namespace Engine::Renderer::RenderFramework::OpenGl
{
    struct GLUploadFormat
    {
        GLint internalFormat;
        GLenum format;
        GLenum type;
    };

    inline GLUploadFormat ToGL(PixelFormat pixel_format)
    {
        switch (pixel_format)
        {
            case PixelFormat::R8:
                return {
                    .internalFormat = GL_R8,
                    .format = GL_RED,
                    .type = GL_UNSIGNED_BYTE
                };
            case PixelFormat::RGB8:
                return {
                    .internalFormat = GL_RGB8,
                    .format = GL_RGB,
                    .type = GL_UNSIGNED_BYTE
                };
            case PixelFormat::RGBA8:
                return {
                    .internalFormat = GL_RGBA8,
                    .format = GL_RGBA,
                    .type = GL_UNSIGNED_BYTE
                };
        }
        throw std::runtime_error("[OpenGL] Unknown pixel format: " + std::to_string(static_cast<int>(pixel_format)));
    }

    struct OpenGLTexture
    {
        GLuint texture_id;
        GLint width;
        GLint height;
        GLUploadFormat uploadFormat;
    };
}
