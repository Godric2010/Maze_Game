//
// Created by Sebastian Borsch on 29.10.25.
//

#pragma once

namespace yarep::renderer
{
    inline asset_handling::MeshAsset CreateUiPrimitive()
    {
        auto ui_mesh_primitive = asset_handling::MeshAsset();

        ui_mesh_primitive.vertices = std::vector{
            asset_handling::MeshVertexAsset{glm::vec3{0, 0, 0}, glm::vec3(0), glm::vec2{0, 0}},
            asset_handling::MeshVertexAsset{glm::vec3{0, 1, 0}, glm::vec3(0), glm::vec2{0, 1}},
            asset_handling::MeshVertexAsset{glm::vec3{1, 1, 0}, glm::vec3(0), glm::vec2{1, 1}},
            asset_handling::MeshVertexAsset{glm::vec3{1, 0, 0}, glm::vec3(0), glm::vec2{1, 0}},
        };

        ui_mesh_primitive.indices = std::vector<uint32_t>{0, 1, 2, 0, 2, 3};
        return ui_mesh_primitive;
    };
}
