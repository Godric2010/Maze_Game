#include "MeshImporter.hpp"

#include <algorithm>
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
            MeshVertex vertex = BuildMeshVertex(vertex_indices[i], vertex_positions, vertex_normals, vertex_uvs);
            vertices.push_back(vertex);

            indices.push_back(i);
        }
    }

    MeshVertex MeshImporter::BuildMeshVertex(const VertexIndices indices, const std::vector<glm::vec3>& vertex_positions,
                                             const std::vector<glm::vec3>& vertex_normals, const std::vector<glm::vec2>& vertex_uvs)
    {
        MeshVertex vertex{};
        if (IsIndexValid("PositionIndex", indices.PositionIndex, vertex_positions.size(), true))
        {
            vertex.position = vertex_positions[indices.PositionIndex - 1];
        }
        if (IsIndexValid("UvIndex", indices.UvIndex, vertex_uvs.size(), false))
        {
            vertex.uv = vertex_uvs[indices.UvIndex - 1];
        }
        if (IsIndexValid("NormalIndex", indices.NormalIndex, vertex_normals.size(), false))
        {
            vertex.normal = vertex_normals[indices.NormalIndex - 1];
        }
        return vertex;
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
        while (std::getline(stream, line_string, '\n'))
        {
            if (!TryGetValidLineString(line_string))
            {
                continue;
            }
            std::istringstream line_stream(line_string);

            std::string prefix;
            line_stream >> prefix;

            if (prefix == "v")
            {
                glm::vec3 pos;
                if (!TryParseVector3(line_stream, pos))
                {
                    throw std::runtime_error("Cannot read vertex position! Invalid line string: '" + line_string + "'");
                }
                vertex_positions.emplace_back(pos);
                continue;
            }
            if (prefix == "vn")
            {
                glm::vec3 normal;
                if (!TryParseVector3(line_stream, normal))
                {
                    throw std::runtime_error("Cannot read vertex normals! Invalid line string: '" + line_string + "'");
                }
                vertex_normals.emplace_back(normal);
                continue;
            }
            if (prefix == "vt")
            {
                glm::vec2 uv;
                if (!TryParseVector2(line_stream, uv))
                {
                    throw std::runtime_error("Cannot read vertex UVs! Invalid line string: '" + line_string + "'");
                }
                vertex_uvs.emplace_back(uv);
                continue;
            }
            if (prefix == "f")
            {
                ParseFace(line_stream, indices);
                continue;
            }
        }
    }

    bool MeshImporter::IsIndexValid(const std::string& name, const uint32_t index, const size_t size, const bool throw_error_at_zero_index)
    {
        if (index == 0)
        {
            if (throw_error_at_zero_index)
            {
                throw std::out_of_range("Invalid " + name + " index: Zero is not valid!");
            }
            return false;
        }
        if (index > size)
        {
            throw std::out_of_range("Invalid " + name + " index: " + std::to_string(index) + " is out of bounds!");
        }
        return true;
    }

    bool MeshImporter::TryGetValidLineString(std::string& line_string)
    {
        if (line_string.empty())
        {
            return false;
        }

        // Handle Windows CRLF: if the line ends with '\r', remove it.
        if (line_string.back() == '\r')
        {
            line_string.pop_back();
        }

        if (const auto hashPos = line_string.find('#'); hashPos != std::string::npos)
        {
            line_string = line_string.substr(0, hashPos);
        }

        auto only_whitespaces = [](const std::string& str)
        {
            return std::ranges::all_of(str, [](const unsigned char c) { return std::isspace(c); });
        };

        if (only_whitespaces(line_string))
        {
            return false;
        }
        return true;
    }

    void MeshImporter::ParseFace(std::istringstream& line_stream, std::vector<VertexIndices>& indices)
    {
        std::vector<std::string> corner_tokens;
        std::string token;
        while (line_stream >> token)
        {
            corner_tokens.push_back(token);
        }

        if (corner_tokens.size() < 3)
        {
            throw std::runtime_error("Cannot have a face with less than three vertices!");
        }
        for (const auto& corner_token : corner_tokens)
        {
            std::istringstream token_stream(corner_token);

            std::string position_token, normal_token, uv_token;

            std::getline(token_stream, position_token, '/');
            std::getline(token_stream, uv_token, '/');
            std::getline(token_stream, normal_token, '/');


            uint32_t position_index = 0, normal_index = 0, uv_index = 0;
            position_index = ParseStringToIndex(position_token, "Position", corner_token);
            if (!normal_token.empty())
            {
                normal_index = ParseStringToIndex(normal_token, "Normal", corner_token);
            }
            if (!uv_token.empty())
            {
                uv_index = ParseStringToIndex(uv_token, "UV", corner_token);
            }
            VertexIndices vertex_indices{};
            vertex_indices.PositionIndex = position_index;
            vertex_indices.NormalIndex = normal_index;
            vertex_indices.UvIndex = uv_index;
            indices.push_back(vertex_indices);
        }
    } // namespace

    bool MeshImporter::TryParseVector3(std::istringstream& line_stream, glm::vec3& result)
    {
        float x, y, z;
        if (!(line_stream >> x >> y >> z))
        {
            return false;
        }
        result = glm::vec3(x, y, z);
        return true;
    }

    bool MeshImporter::TryParseVector2(std::istringstream& line_stream, glm::vec2& result)
    {
        float x, y;
        if (!(line_stream >> x >> y))
        {
            return false;
        }
        result = glm::vec2(x, y);
        return true;
    }

    uint32_t MeshImporter::ParseStringToIndex(const std::string& token, const std::string& field_name,
                                              const std::string& full_token)
    {
        try
        {
            const int value = std::stoi(token);
            if (value < 0)
            {
                throw std::runtime_error("Negative index numbers are not supported!");
            }
            return static_cast<uint32_t>(value);
        }
        catch (const std::invalid_argument&)
        {
            throw std::runtime_error("Invalid " + field_name + " index in token '" + full_token + "'");
        }
        catch (const std::out_of_range&)
        {
            throw std::runtime_error("Out of range " + field_name + " index in token '" + full_token + "'");
        }
    }
}
