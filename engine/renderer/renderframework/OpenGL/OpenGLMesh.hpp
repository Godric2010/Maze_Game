//
// Created by Sebastian Borsch on 14.08.25.
//

#pragma once

namespace Engine::Renderer::RenderFramework::OpenGL {
    struct OpenGLMesh {
        unsigned int VAO;
        unsigned int VBO;
        unsigned int EBO;
        uint32_t numVertices;
        uint32_t numIndices;
    };
}
