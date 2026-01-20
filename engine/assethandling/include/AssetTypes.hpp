#pragma once
#include <string>
#include <vector>
#include <glm/glm.hpp>

namespace Engine::AssetHandling {
    struct Asset {
        virtual ~Asset() = default;
    };

    template<typename T>
    concept AssetType = std::is_base_of_v<Asset, T>;

    struct ShaderAsset final : Asset {
        std::string name;
        std::string vertex_content;
        std::string fragment_content;
    };

    struct FontAsset : Asset {
        std::string name;
        std::vector<uint8_t> bytes;
    };

    struct MeshVertex {
        glm::vec3 position;
        glm::vec3 normal;
        glm::vec2 uv;
    };

    struct MeshAsset : Asset {
        std::vector<MeshVertex> vertices;
        std::vector<uint32_t> indices;
    };
}
