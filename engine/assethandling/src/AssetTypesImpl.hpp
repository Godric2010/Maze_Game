//
// Created by Sebastian Borsch on 18.01.26.
//

#pragma once
#include <stdexcept>

#include "IFileReader.hpp"
#include "../include/AssetTypes.hpp"
#include "Mesh/MeshImporter.hpp"

namespace Engine::AssetHandling {
    template<typename T>
    struct AssetTraits;

    template<>
    struct AssetTraits<ShaderAsset> {
        inline static const std::string dir_name = std::string("shaders");

        static std::shared_ptr<ShaderAsset> Load(Environment::Files::IFileReader* file_reader,
                                                 const std::string& asset_name) {
            const auto vertex_shader_content = file_reader->ReadTextFromFile(dir_name + "/" + asset_name + ".vert");
            if (!vertex_shader_content.Ok()) {
                throw std::runtime_error("Failed to load vertex shader" + asset_name);
            }
            const auto fragment_shader_content = file_reader->ReadTextFromFile(dir_name + "/" + asset_name + ".frag");
            if (!fragment_shader_content.Ok()) {
                throw std::runtime_error("Failed to load fragment shader" + asset_name);
            }
            auto shader_asset = std::make_shared<ShaderAsset>();
            shader_asset->name = asset_name;
            shader_asset->vertex_content = vertex_shader_content.value;
            shader_asset->fragment_content = fragment_shader_content.value;
            return shader_asset;
        }
    };

    template<>
    struct AssetTraits<FontAsset> {
        inline static const std::string dir_name = std::string("fonts");

        static std::shared_ptr<FontAsset> Load(Environment::Files::IFileReader* file_reader,
                                               const std::string& asset_name) {
            const auto font_content = file_reader->ReadBinaryFromFile(dir_name + "/" + asset_name);
            if (!font_content.Ok()) {
                throw std::runtime_error("Failed to load font asset" + asset_name);
            }
            const auto font_asset = std::make_shared<FontAsset>();
            font_asset->name = asset_name;
            font_asset->bytes = font_content.value.data;
            return font_asset;
        }
    };

    template<>
    struct AssetTraits<MeshAsset> {
        inline static const std::string dir_name = std::string("meshes");

        static std::shared_ptr<MeshAsset> Load(Environment::Files::IFileReader* file_reader,
                                               const std::string& asset_name) {
            const auto mesh_content = file_reader->ReadTextFromFile(dir_name + "/" + asset_name);
            if (!mesh_content.Ok()) {
                throw std::runtime_error("Failed to load mesh asset" + asset_name);
            }

            std::vector<MeshVertex> vertices;
            std::vector<uint32_t> indices;
            Mesh::MeshImporter::BuildMeshAssetFromObj(mesh_content.value, vertices, indices);
            auto mesh_asset = std::make_shared<MeshAsset>();
            mesh_asset->vertices = vertices;
            mesh_asset->indices = indices;
            return mesh_asset;
        }
    };
}
