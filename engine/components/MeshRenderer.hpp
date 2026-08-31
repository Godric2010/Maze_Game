//
// Created by Sebastian Borsch on 25.08.25.
//

#pragma once
#include <glm/glm.hpp>
#include "../renderer/include/Datatypes.hpp"

namespace yarep::components
{
    struct MeshRenderer
    {
        assets::MeshHandle mesh;
        assets::MaterialHandle material;
    };
}
