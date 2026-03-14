//
// Created by Sebastian Borsch on 14.08.25.
//

#pragma once
#include <vector>
#include <glm/fwd.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "AssetTypes.hpp"
#include "Assets/AssetHandleTypes.hpp"
#include "Ecs/Types.hpp"

namespace Engine::Renderer
{
    struct CameraAsset
    {
        glm::mat4 view;
        glm::mat4 projection;
        glm::vec4 camera_position;
    };


    struct MeshDrawAsset
    {
        Ecs::EntityId Entity;
        AssetHandling::RenderState RenderState;
        size_t RenderQueueIndex;
        Assets::MeshHandle Mesh;
        Assets::MaterialHandle Material;
        glm::mat4 Model;
    };

    struct UiDrawAsset
    {
        glm::mat4 model;
        Assets::MeshHandle mesh;
        Assets::MaterialHandle material;
        float layer;
        glm::vec4 color;
    };

    struct DrawAssets
    {
        std::vector<MeshDrawAsset> mesh_draw_assets;
        std::vector<UiDrawAsset> ui_draw_assets;
    };
}
