#pragma once
#include <string>
#include <vector>
#include "AssetTypes.hpp"

namespace Engine::AssetHandling::Mesh {
    class MeshImporter {
    public:
        static void BuildMeshAssetFromObj(const std::string& obj_string,
                                          std::vector<MeshVertex>& vertices,
                                          std::vector<uint32_t>& indices);

    private:
        struct VertexIndices
        {
            uint32_t PositionIndex;
            uint32_t NormalIndex;
            uint32_t UvIndex;
        };

        static void AnalyseString(const std::string& str, std::vector<glm::vec3>& vertex_positions,
                                  std::vector<glm::vec3>& vertex_normals, std::vector<glm::vec2>& vertex_uvs,
                                  std::vector<VertexIndices>& indices);
    };
} // namespace
