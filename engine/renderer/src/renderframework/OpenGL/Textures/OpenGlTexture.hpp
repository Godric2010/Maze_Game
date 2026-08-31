//
// Created by Sebastian Borsch on 16.11.25.
//

#pragma once
#include <GL/glew.h>
#include <stdexcept>
#include <string>

#include "AssetTypes.hpp"

namespace yarep::renderer::render_framework::open_gl
{
    struct GlUploadFormat
    {
        GLint internal_format;
        GLenum format;
        GLenum type;
    };

    inline GlUploadFormat ToGl(asset_handling::PixelFormat pixel_format)
    {
        switch (pixel_format)
        {
            case asset_handling::PixelFormat::R8:
                return {
                    .internal_format = GL_R8,
                    .format = GL_RED,
                    .type = GL_UNSIGNED_BYTE
                };
            case asset_handling::PixelFormat::Rgb8:
                return {
                    .internal_format = GL_RGB8,
                    .format = GL_RGB,
                    .type = GL_UNSIGNED_BYTE
                };
            case asset_handling::PixelFormat::Rgba8:
                return {
                    .internal_format = GL_RGBA8,
                    .format = GL_RGBA,
                    .type = GL_UNSIGNED_BYTE
                };
        }
        throw std::runtime_error("[OpenGL] Unknown pixel format: " + std::to_string(static_cast<int>(pixel_format)));
    }

    struct OpenGlTexture
    {
        GLuint texture_id;
        GLint width;
        GLint height;
        GlUploadFormat upload_format;
    };
}
