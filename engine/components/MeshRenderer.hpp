//
// Created by Sebastian Borsch on 25.08.25.
//

#pragma once
#include <glm/glm.hpp>
#include "../renderer/include/Datatypes.hpp"

namespace Engine::Components
{
    struct MeshRenderer
    {
        Assets::MeshHandle mesh;
        Assets::TextureHandle texture;
        glm::vec4 color;
    };
}
