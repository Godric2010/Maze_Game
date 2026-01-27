#include "MeshImporter.hpp"

#include <iostream>
#include <sstream>

namespace Engine::AssetHandling::Mesh
{
    void MeshImporter::BuildMeshAssetFromObj(const std::string& obj_string,
                                             std::vector<MeshVertex>& vertices,
                                             std::vector<uint32_t>& indices)
    {
        std::vector<glm::vec3> vertex_positions;
        std::vector<glm::vec3> vertex_normals;
        std::vector<glm::vec2> vertex_uvs;
        std::vector<VertexIndices> vertex_indices;
        AnalyseString(obj_string, vertex_positions, vertex_normals, vertex_uvs, vertex_indices);


        for (uint32_t i = 0; i < vertex_indices.size(); ++i)
        {
            const auto [PositionIndex, NormalIndex, UvIndex] = vertex_indices[i];

            MeshVertex vertex{};
            vertex.position = vertex_positions[PositionIndex - 1];
            if (NormalIndex != 0)
            {
                vertex.normal = vertex_normals[NormalIndex - 1];
            }
            if (UvIndex != 0)
            {
                vertex.uv = vertex_uvs[UvIndex - 1];
            }
            vertices.push_back(vertex);
            indices.push_back(i);
        }
    }

    void MeshImporter::AnalyseString(const std::string& str, std::vector<glm::vec3>& vertex_positions,
                                     std::vector<glm::vec3>& vertex_normals, std::vector<glm::vec2>& vertex_uvs,
                                     std::vector<VertexIndices>& indices)
    {
        std::stringstream stream(str);
        if (str.empty())
        {
            throw std::invalid_argument("The string cannot be empty");
        }

        std::string line_string;
        std::string line_token;
        std::vector<std::string> line_tokens;
        while (std::getline(stream, line_string, '\n'))
        {
            line_tokens.clear();
            line_token.clear();

            if (line_string.empty())
            {
                continue;
            }
            std::istringstream line_stream(line_string);

            while (line_stream >> line_token)
            {
                line_tokens.push_back(line_token);
            }
            if (line_tokens.empty())
            {
                continue;
            }

            auto prefix = line_tokens.front();
            if (prefix == "#")
            {
                continue;
            }
            if (prefix == "v")
            {
                float x, y, z;
                std::istringstream token_stream_x(line_tokens[1]);
                token_stream_x >> x;

                std::istringstream token_stream_y(line_tokens[2]);
                token_stream_y >> y;

                std::istringstream token_stream_z(line_tokens[3]);
                token_stream_z >> z;
                vertex_positions.emplace_back(x, y, z);
                std::cout << "Vertex Pos:" << x << ", " << y << ", " << z << std::endl;
                continue;
            }
            if (prefix == "vn")
            {
                float x, y, z;
                std::istringstream token_stream_x(line_tokens[1]);
                token_stream_x >> x;
                std::istringstream token_stream_y(line_tokens[2]);
                token_stream_y >> y;
                std::istringstream token_stream_z(line_tokens[3]);
                token_stream_z >> z;
                vertex_normals.emplace_back(x, y, z);
                std::cout << "Vertex Normal:" << x << ", " << y << ", " << z << std::endl;
                continue;
            }
            if (prefix == "vt")
            {
                float x, y;
                std::istringstream token_stream_x(line_tokens[1]);
                token_stream_x >> x;
                std::istringstream token_stream_y(line_tokens[2]);
                token_stream_y >> y;
                vertex_uvs.emplace_back(x, y);
                std::cout << "Vertex UV:" << x << ", " << y << std::endl;
                continue;
            }
            if (prefix == "f")
            {
                if (line_tokens.size() < 4)
                {
                    throw std::runtime_error("Cannot have a face with less than three vertices!");
                }
                for (uint i = 1; i < line_tokens.size(); ++i)
                {
                    std::istringstream token_stream(line_tokens[i]);

                    std::string position_token;
                    std::string normal_token;
                    std::string uv_token;

                    std::getline(token_stream, position_token, '/');
                    std::getline(token_stream, uv_token, '/');
                    std::getline(token_stream, normal_token, '/');

                    uint32_t position_index = 0, normal_index = 0, uv_index = 0;
                    position_index = std::stoi(position_token);
                    if (!normal_token.empty())
                    {
                        normal_index = std::stoi(normal_token);
                    }
                    if (!uv_token.empty())
                    {
                        uv_index = std::stoi(uv_token);
                    }
                    VertexIndices vertex_indices{};
                    vertex_indices.PositionIndex = position_index;
                    vertex_indices.NormalIndex = normal_index;
                    vertex_indices.UvIndex = uv_index;
                    indices.push_back(vertex_indices);
                }
                continue;
            }
        }
    }
} // namespace
