//
// Created by Sebastian Borsch on 29.10.25.
//

#pragma once
#include "Datatypes.hpp"

namespace Engine::Renderer {
    inline MeshAsset CreateUiPrimitive() {
        MeshAsset ui_mesh_primitive{};

        ui_mesh_primitive.vertices = std::vector{
            glm::vec3{0, 0, 0},
            glm::vec3{1, 0, 0},
            glm::vec3{1, 1, 0},
            glm::vec3{0, 1, 0}
        };

        ui_mesh_primitive.indices = std::vector<uint32_t>{0, 1, 2, 0, 2, 3};
        return ui_mesh_primitive;
    };
}
