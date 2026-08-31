//
// Created by Sebastian Borsch on 14.08.25.
//

#pragma once

namespace yarep::renderer::render_framework::open_gl
{
    struct OpenGlMesh
    {
        unsigned int vao;
        unsigned int vbo;
        unsigned int ebo;
        uint32_t num_vertices;
        uint32_t num_indices;

        bool IsValid() const
        {
            return num_vertices >= 3 && num_indices >= 3;
        }
    };
}
