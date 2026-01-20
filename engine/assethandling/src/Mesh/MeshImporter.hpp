#pragma once
#include "AssetTypes.hpp"

namespace Engine::Assethandling::Mesh {
    class MeshImporter {
    public:
        static void BuildMeshAssetFromObj(const std::string& obj_string,
                                          std::vector<AssetHandling::MeshVertex>& vertices,
                                          std::vector<uint32_t>& indices);
    };
} // namespace
