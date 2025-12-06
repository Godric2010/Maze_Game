//
// Created by Sebastian Borsch on 25.08.25.
//

#pragma once
#include <glm/glm.hpp>
#include "../renderer/Datatypes.hpp"

namespace Engine::Components{
    struct Mesh {
        Renderer::MeshHandle mesh;
        glm::vec4 color;
    };
}
