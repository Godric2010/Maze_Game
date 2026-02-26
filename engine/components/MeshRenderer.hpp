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
        Assets::MeshHandle Mesh;
        Assets::MaterialHandle Material;
    };
}
