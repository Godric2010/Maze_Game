#pragma once
#include <string>
#include <vector>
#include "AssetTypes.hpp"

namespace yarep::asset_handling::mesh
{
    class MeshImporter
    {
    public:
        static void BuildMeshAssetFromObj(const std::string& obj_string,
                                          std::vector<MeshVertexAsset>& vertices,
                                          std::vector<uint32_t>& indices);
    private:
        struct FaceVertexIndex
        {
            uint32_t position_index;
            uint32_t normal_index;
            uint32_t uv_index;

            bool operator==(const FaceVertexIndex& other) const
            {
                return position_index == other.position_index && other.normal_index == normal_index && uv_index == other.
                    uv_index;
            }
        };
        
        struct FaceVertexIndexHasher
        {
           size_t operator()(const FaceVertexIndex& index) const noexcept
           {
               size_t pos_hash = std::hash<uint32_t>{}(index.position_index);
               size_t normal_hash = std::hash<uint32_t>{}(index.normal_index);
               size_t uv_hash = std::hash<uint32_t>{}(index.uv_index);
               size_t seed = pos_hash;
               seed ^= normal_hash + 0x9e3779b9 + (seed << 6) + (seed >> 2);
               seed ^= uv_hash + 0x9e3779b9 + (seed << 6) + (seed >> 2);
               return seed;
           } 
        };

        struct Face
        {
            std::vector<FaceVertexIndex> indices;
        };

        static void AnalyseString(const std::string& str, std::vector<glm::vec3>& vertex_positions,
                                  std::vector<glm::vec3>& vertex_normals, std::vector<glm::vec2>& vertex_uvs,
                                  std::vector<Face>& face);


        static MeshVertexAsset BuildMeshVertex(FaceVertexIndex indices, const std::vector<glm::vec3>& vertex_positions,
                                          const std::vector<glm::vec3>& vertex_normals,
                                          const std::vector<glm::vec2>& vertex_uvs);

        static bool IsIndexValid(const std::string& name, uint32_t index, size_t size, bool throw_error_at_zero_index);
        static bool TryGetValidLineString(std::string& line_string);
        static void ParseFace(std::istringstream& line_stream, std::vector<Face>& faces);
        static void FanTriangulateFace(Face& face);
        static bool TryParseVector3(std::istringstream& line_stream, glm::vec3& result);
        static bool TryParseVector2(std::istringstream& line_stream, glm::vec2& result);
        static uint32_t ParseStringToIndex(const std::string& token, const std::string& field_name,
                                           const std::string& full_token);
    };
} // namespace
