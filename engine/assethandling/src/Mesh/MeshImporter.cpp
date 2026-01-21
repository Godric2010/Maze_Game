#include "MeshImporter.hpp"

#include <iostream>
#include <sstream>

namespace Engine::Assethandling::Mesh
{
    void MeshImporter::BuildMeshAssetFromObj(const std::string& obj_string,
                                             std::vector<AssetHandling::MeshVertex>& vertices,
                                             std::vector<uint32_t>& indices)
    {
        std::vector<glm::vec3> vertex_positions;
        AnalyseString(obj_string, vertex_positions, indices);

        for (const auto vertex_position : vertex_positions)
        {
            vertices.push_back(AssetHandling::MeshVertex{
                    .position = vertex_position,
                    .normal = glm::vec3(0.0f, 0.0f, 0.0f),
                    .uv = glm::vec2(0.0f, 0.0f),
                }
            );
        }
    }

    void MeshImporter::AnalyseString(const std::string& str, std::vector<glm::vec3>& vertex_positions,
                                     std::vector<uint32_t>& indices)
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
                std::cout << "Vertex:" << x << ", " << y << ", " << z << std::endl;
                continue;
            }
            if (prefix == "f")
            {
                std::cout << "Face:";
                for (uint i = 1; i < line_tokens.size(); ++i)
                {
                    int index;
                    std::istringstream token_stream(line_tokens[i]);
                    token_stream >> index;
                    indices.push_back(--index);
                    std::cout << index << ", ";
                }
                std::cout << std::endl;
                continue;
            }
        }
    }
} // namespace
