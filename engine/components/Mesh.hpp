//
// Created by Sebastian Borsch on 25.08.25.
//

#pragma once
#include <glm/glm.hpp>
#include "../renderer/Datatypes.hpp"

namespace Engine::Core::Components{
    struct Mesh {
        Renderer::MeshHandle mesh;
        glm::vec4 color;
    };
}
