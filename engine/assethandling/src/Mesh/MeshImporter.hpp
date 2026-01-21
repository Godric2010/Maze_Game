#pragma once
#include <string>
#include <vector>
#include "AssetTypes.hpp"

namespace Engine::Assethandling::Mesh {
    class MeshImporter {
    public:
        static void BuildMeshAssetFromObj(const std::string& obj_string,
                                          std::vector<AssetHandling::MeshVertex>& vertices,
                                          std::vector<uint32_t>& indices);

    private:
        static void AnalyseString(const std::string& str, std::vector<glm::vec3>& vertex_positions,
                                  std::vector<uint32_t>& indices);
    };
} // namespace
