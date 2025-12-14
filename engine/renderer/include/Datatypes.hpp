//
// Created by Sebastian Borsch on 14.08.25.
//

#pragma once
#include <vector>
#include <glm/fwd.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/vec3.hpp>

namespace Engine::Renderer {
    typedef uint32_t MeshHandle;
    typedef uint32_t TextureHandle;

    struct MeshVertex {
        glm::vec3 position;
        glm::vec2 uv;
    };

    struct MeshAsset {
        std::vector<MeshVertex> vertices;
        std::vector<uint32_t> indices;
    };

    struct TextureAsset {
        float width;
        float height;
        std::vector<uint8_t> pixels;
    };

    struct CameraAsset {
        glm::mat4 view;
        glm::mat4 projection;
        glm::vec4 cameraPosition;
    };


    struct MeshDrawAsset {
        MeshHandle mesh;
        glm::mat4 model;
        glm::vec4 color;
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
