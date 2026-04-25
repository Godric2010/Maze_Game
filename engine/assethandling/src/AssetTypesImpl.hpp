//
// Created by Sebastian Borsch on 18.01.26.
//

#pragma once
#include <ranges>
#include <stdexcept>

#include "IFileManager.hpp"
#include "../include/AssetTypes.hpp"
#include "Assets/AssetHandleTypes.hpp"
#include "InputMaps/InputMapImporter.hpp"
#include "Materials/MaterialImporter.hpp"
#include "Mesh/MeshImporter.hpp"
#include "Textures/TextureImporter.hpp"

namespace Engine::AssetHandling {
    using namespace Assets;
    template<typename T>
    struct AssetTraits;

    template<>
    struct AssetTraits<ShaderAsset> {
        using Handle = ShaderHandle;
        inline static const std::string dir_name = std::string("shaders");

        static std::vector<ShaderAsset> LoadMany(const AssetLoadContext context,
                                                 const std::vector<Environment::Files::FilePath>& file_paths) {
            std::unordered_map<std::string, ShaderAsset> shader_name_content_map;
            for (int i = 0; i < file_paths.size(); ++i) {
                const std::string extension = file_paths[i].file_type;
                const std::string file = file_paths[i].relative_file_path;
                const std::string file_name = file.substr(0, file.size() - extension.size());

                const std::string file_content = GetShaderFileContent(context, file);
                if (!shader_name_content_map.contains(file_name)) {
                    auto shader_asset = ShaderAsset();
                    shader_asset.name = file_name;
                    shader_name_content_map[file_name] = shader_asset;
                }

                if (extension == ".vert") {
                    shader_name_content_map[file_name].vertex_content = file_content;
                } else if (extension == ".frag") {
                    shader_name_content_map[file_name].fragment_content = file_content;
                } else if (extension == ".glsl") {
                } else {
                    throw std::runtime_error("Unknown shader file extension: " + extension);
                }
            }
            std::vector<ShaderAsset> shader_assets;
            shader_assets.reserve(shader_name_content_map.size());
            for (const auto& shader_name: shader_name_content_map | std::ranges::views::values) {
                shader_assets.emplace_back(shader_name);
            }
            return shader_assets;
        }

    private:
        static std::string GetShaderFileContent(const AssetLoadContext context, const std::string& file) {
            const std::string path = dir_name + "/" + file;
            auto content = context.file_reader->ReadTextFromResourceFile(path);
            if (!content.Ok()) {
                throw std::runtime_error("Failed to load shader file " + path);
            }
            return content.value;
        }
    };

    template<>
    struct AssetTraits<FontAsset> {
        using Handle = FontHandle;
        inline static const std::string dir_name = std::string("fonts");

        static FontAsset Load(AssetLoadContext context,
                              const std::string& asset_name) {
            const auto font_content = context.file_reader->ReadBinaryFromResourceFile(dir_name + "/" + asset_name);
            if (!font_content.Ok()) {
                throw std::runtime_error("Failed to load font asset" + asset_name);
            }
            auto font_asset = FontAsset();
            font_asset.name = asset_name;
            font_asset.bytes = font_content.value.data;
            return font_asset;
        }
    };

    template<>
    struct AssetTraits<MeshAsset> {
        using Handle = MeshHandle;
        inline static const std::string dir_name = std::string("meshes");

        static MeshAsset Load(AssetLoadContext context,
                              const std::string& asset_name) {
            const auto mesh_content = context.file_reader->ReadTextFromResourceFile(dir_name + "/" + asset_name);
            if (!mesh_content.Ok()) {
                throw std::runtime_error("Failed to load mesh asset " + asset_name);
            }

            std::vector<MeshVertexAsset> vertices;
            std::vector<uint32_t> indices;
            Mesh::MeshImporter::BuildMeshAssetFromObj(mesh_content.value, vertices, indices);
            auto mesh_asset = MeshAsset();
            mesh_asset.vertices = vertices;
            mesh_asset.indices = indices;
            return mesh_asset;
        }
    };

    template<>
    struct AssetTraits<TextureAsset> {
        using Handle = TextureHandle;
        inline static const std::string dir_name = std::string("textures");

        static TextureAsset Load(AssetLoadContext context,
                                 const std::string& asset_name) {
            const auto texture_content = context.file_reader->ReadBinaryFromResourceFile(dir_name + "/" + asset_name);
            if (!texture_content.Ok()) {
                throw std::runtime_error("Failed to load texture asset " + asset_name);
            }
            auto texture_asset = TextureAsset();
            Textures::TextureImporter::BuildTextureFromFile(texture_asset, texture_content.value.data, asset_name);
            return texture_asset;
        }
    };

    template<>
    struct AssetTraits<MaterialAsset> {
        using Handle = MaterialHandle;
        inline static const std::string dir_name = std::string("materials");

        static MaterialAsset Load(AssetLoadContext context,
                                  const std::string& asset_name) {
            const auto toml_file_content = context.file_reader->ReadTextFromResourceFile(dir_name + "/" + asset_name);
            if (!toml_file_content.Ok()) {
                throw std::runtime_error("Failed to load material asset " + asset_name);
            }
            Materials::MaterialFileData material_file_data{};
            Materials::MaterialImporter::ExtractMaterialFileData(material_file_data, toml_file_content.value);

            auto shader_handle = context.asset_handler->FindShader(material_file_data.shader_name);
            if (!shader_handle.has_value()) {
                throw std::runtime_error("Material shader " + material_file_data.shader_name + " not found");
            }

            auto material = MaterialAsset();
            material.name = material_file_data.name;
            material.render_state = material_file_data.render_state;
            material.render_queue_index = material_file_data.render_queue_index;
            material.shader_handle = shader_handle.value();
            material.base_color = material_file_data.base_color;
            material.albedo_texture.texture = context.asset_handler->
                    LoadTexture(material_file_data.albedo_texture.name);
            material.albedo_texture.tiling = material_file_data.albedo_texture.tiling;
            material.albedo_texture.uv_scale = material_file_data.albedo_texture.uv_scale;


            return material;
        }
    };

    template<>
    struct AssetTraits<InputMapAsset> {
        using Handle = InputMapHandle;
        inline static const auto dir_name = std::string("inputmaps");

        static std::vector<InputMapAsset> LoadMany(const AssetLoadContext context,
                                                   const std::vector<Environment::Files::FilePath>& file_paths) {
            std::vector<InputMapAsset> input_maps;
            input_maps.reserve(file_paths.size());
            for (const auto& file_path: file_paths) {
                const std::string path = dir_name + "/" + file_path.relative_file_path;
                const auto toml_file_content = context.file_reader->ReadTextFromResourceFile(path);
                if (!toml_file_content.Ok()) {
                    throw std::runtime_error("Failed to load input map asset " + path);
                }
                const auto input_map_asset = LoadInputMap(toml_file_content.value);
                input_maps.push_back(input_map_asset);
            }
            return input_maps;
        }

    private:
        static InputMapAsset LoadInputMap(const std::string& toml_file_content) {
            Input::InputMap input_map;
            InputMaps::InputMapImporter::ExtractInputMapFromFileData(input_map, toml_file_content);
            auto input_map_asset = InputMapAsset();
            input_map_asset.name = input_map.name;
            input_map_asset.input_map = input_map;
            return input_map_asset;
        }
    };
}
