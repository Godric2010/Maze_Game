//
// Created by Sebastian Borsch on 14.08.25.
//

#pragma once
#include <vector>
#include <glm/fwd.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Renderer/Types.hpp"

namespace Engine::Renderer {

    struct CameraAsset {
        glm::mat4 view;
        glm::mat4 projection;
        glm::vec4 camera_position;
    };


    struct MeshDrawAsset {
        MeshHandle mesh;
        glm::mat4 model;
        glm::vec4 color;
        TextureHandle texture;
    };

    struct UiDrawAsset {
        glm::mat4 model;
        MeshHandle mesh;
        TextureHandle texture;
        float layer;
        glm::vec4 color;
    };

    struct DrawAssets {
        std::vector<MeshDrawAsset> mesh_draw_assets;
        std::vector<UiDrawAsset> ui_draw_assets;
    };
}
