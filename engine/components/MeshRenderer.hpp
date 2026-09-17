//
// Created by Sebastian Borsch on 25.08.25.
//

#pragma once
#include "Assets/AssetHandleTypes.hpp"

namespace yarep::components
{
    struct MeshRenderer
    {
        assets::MeshHandle mesh;
        assets::MaterialHandle material;
    };
}
