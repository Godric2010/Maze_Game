//
// Created by Sebastian Borsch on 16.11.25.
//

#pragma once
#include <glad/glad.h>

namespace Engine::Renderer::RenderFramework::OpenGl {
    struct OpenGLTexture {
        GLuint texture_id;
        uint32_t width;
        uint32_t height;
    };
}
