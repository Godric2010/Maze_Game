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
#include "Shader/ShaderPreProcessor.hpp"
#include "Textures/TextureImporter.hpp"

namespace Engine::AssetHandling {
    using namespace Assets;
    template<typename T>
    struct AssetTraits;

    template<>
    struct AssetTraits<ShaderAsset> {
        using Handle = ShaderHandle;
        inline static const auto dir_name = std::string("shaders");

        static std::vector<ShaderAsset> LoadMany(const AssetLoadContext context,
                                                 const std::vector<Environment::Files::File>& file_paths) {
            std::unordered_map<std::string, std::string> vert_shader_content_map;
            std::unordered_map<std::string, std::string> frag_shader_content_map;
            std::unordered_map<std::string, std::string> helper_shader_content_map;

            for (int i = 0; i < file_paths.size(); ++i) {
                const std::string extension = file_paths[i].extension;
                const std::string file_name = file_paths[i].name;
                const std::string file_content = GetShaderFileContent(context, file_name + extension);
                if (extension == ".vert") {
                    vert_shader_content_map[file_name] = file_content;
                } else if (extension == ".frag") {
                    frag_shader_content_map[file_name] = file_content;
                } else if (extension == ".glsl") {
                    helper_shader_content_map[file_name] = file_content;
                } else {
                    throw std::runtime_error("Unknown shader file extension: " + extension);
                }
            }
            if (vert_shader_content_map.size() != frag_shader_content_map.size()) {
                throw std::runtime_error("Vertex and fragment shader files do not match in amount!");
            }

            auto pre_processor = Shader::ShaderPreProcessor(helper_shader_content_map);
            ProcessShaders(pre_processor, vert_shader_content_map);
            ProcessShaders(pre_processor, frag_shader_content_map);

            std::vector<ShaderAsset> shader_assets;
            shader_assets.reserve(vert_shader_content_map.size());
            for (const auto& [name, vert_content]: vert_shader_content_map) {
                auto shader_asset = ShaderAsset();
                shader_asset.name = name;
                shader_asset.vertex_content = vert_content;
                shader_asset.fragment_content = frag_shader_content_map[name];

                shader_assets.emplace_back(shader_asset);
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

        static void ProcessShaders(Shader::ShaderPreProcessor& pre_processor,
                                   std::unordered_map<std::string, std::string>& shader_content_map) {
            for (auto& [shader_name, content]: shader_content_map) {
                const auto original = content;
                const auto processed = pre_processor.PreProcessShader(original);
                shader_content_map[shader_name] = processed;
            }
        }
    };

    template<>
    struct AssetTraits<FontAsset> {
        using Handle = FontHandle;
        inline static const auto dir_name = std::string("fonts");

        static FontAsset Load(const AssetLoadContext context,
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
        inline static const auto dir_name = std::string("meshes");

        static MeshAsset Load(const AssetLoadContext context,
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
        inline static const auto dir_name = std::string("textures");

        static TextureAsset Load(const AssetLoadContext context,
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
        inline static const auto dir_name = std::string("materials");

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
                                                   const std::vector<Environment::Files::File>& files) {
            std::vector<InputMapAsset> input_maps;
            input_maps.reserve(files.size());
            for (const auto& [extension, name]: files) {
                const std::string path = dir_name + "/" + name + extension;
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
