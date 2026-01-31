#pragma once
#include <string>
#include <vector>
#include "AssetTypes.hpp"

namespace Engine::AssetHandling::Mesh
{
    class MeshImporter
    {
    public:
        static void BuildMeshAssetFromObj(const std::string& obj_string,
                                          std::vector<MeshVertex>& vertices,
                                          std::vector<uint32_t>& indices);
        static void ParseFace(std::istringstream line_stream);

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


        static MeshVertex BuildMeshVertex(VertexIndices indices, const std::vector<glm::vec3>& vertex_positions,
                                          const std::vector<glm::vec3>& vertex_normals, const std::vector<glm::vec2>& vertex_uvs);
        
        static bool IsIndexValid(const std::string& name, uint32_t index, size_t size, bool throw_error_at_zero_index);
        static bool TryGetValidLineString(std::string& line_string);
        static void ParseFace(std::istringstream& line_stream, std::vector<VertexIndices>& indices);
        static bool TryParseVector3(std::istringstream& line_stream, glm::vec3& result);
        static bool TryParseVector2(std::istringstream& line_stream, glm::vec2& result);
        static uint32_t ParseStringToIndex(const std::string& token, const std::string& field_name, const std::string& full_token);
    };
} // namespace
