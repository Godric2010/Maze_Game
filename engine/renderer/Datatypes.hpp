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

    struct MeshAsset {
        std::vector<glm::vec3> vertices;
        std::vector<uint32_t> indices;
    };

    struct CameraAsset {
        glm::mat4 view;
        glm::mat4 projection;
        glm::vec4 cameraPosition;
    };

    struct DrawAsset {
        MeshHandle mesh;
        glm::mat4 model;
        glm::vec4 color;
    };
}
