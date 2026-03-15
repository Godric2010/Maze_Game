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

    struct DrawAsset
    {
        Ecs::EntityId Entity;
        AssetHandling::RenderState RenderState;
        size_t RenderQueueIndex;
        Assets::MeshHandle Mesh;
        Assets::MaterialHandle Material;
        glm::mat4 Model;
        glm::vec4 Color;
    };
}
