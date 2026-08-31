//
// Created by Sebastian Borsch on 25.08.25.
//

#pragma once
#include <glm/glm.hpp>
#include "../renderer/include/Datatypes.hpp"

namespace yarep::Components
{
    struct MeshRenderer
    {
        assets::MeshHandle Mesh;
        assets::MaterialHandle Material;
    };
}
