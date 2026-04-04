//
// Created by Sebastian Borsch on 18.01.26.
//

#pragma once
#include <stdexcept>

#include "IFileReader.hpp"
#include "../include/AssetTypes.hpp"
#include "Assets/AssetHandleTypes.hpp"
#include "InputMaps/InputMapImporter.hpp"
#include "Materials/MaterialImporter.hpp"
#include "Mesh/MeshImporter.hpp"
#include "Textures/TextureImporter.hpp"

namespace Engine::AssetHandling
{
    using namespace Assets;
    template <typename T>
    struct AssetTraits;

    template <>
    struct AssetTraits<ShaderAsset>
    {
        using Handle = ShaderHandle;
        inline static const std::string dir_name = std::string("shaders");

        static ShaderAsset Load(AssetLoadContext context,
                                const std::string& asset_name)
        {
            const auto vertex_shader_content = context.file_reader->
                                                       ReadTextFromFile(dir_name + "/" + asset_name + ".vert");
            if (!vertex_shader_content.Ok())
            {
                throw std::runtime_error("Failed to load vertex shader" + asset_name);
            }
            const auto fragment_shader_content = context.file_reader->
                                                         ReadTextFromFile(dir_name + "/" + asset_name + ".frag");
            if (!fragment_shader_content.Ok())
            {
                throw std::runtime_error("Failed to load fragment shader" + asset_name);
            }
            auto shader_asset = ShaderAsset();
            shader_asset.name = asset_name;
            shader_asset.vertex_content = vertex_shader_content.value;
            shader_asset.fragment_content = fragment_shader_content.value;
            return shader_asset;
        }
    };

    template <>
    struct AssetTraits<FontAsset>
    {
        using Handle = FontHandle;
        inline static const std::string dir_name = std::string("fonts");

        static FontAsset Load(AssetLoadContext context,
                              const std::string& asset_name)
        {
            const auto font_content = context.file_reader->ReadBinaryFromFile(dir_name + "/" + asset_name);
            if (!font_content.Ok())
            {
                throw std::runtime_error("Failed to load font asset" + asset_name);
            }
            auto font_asset = FontAsset();
            font_asset.name = asset_name;
            font_asset.bytes = font_content.value.data;
            return font_asset;
        }
    };

    template <>
    struct AssetTraits<MeshAsset>
    {
        using Handle = MeshHandle;
        inline static const std::string dir_name = std::string("meshes");

        static MeshAsset Load(AssetLoadContext context,
                              const std::string& asset_name)
        {
            const auto mesh_content = context.file_reader->ReadTextFromFile(dir_name + "/" + asset_name);
            if (!mesh_content.Ok())
            {
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

    template <>
    struct AssetTraits<TextureAsset>
    {
        using Handle = TextureHandle;
        inline static const std::string dir_name = std::string("textures");

        static TextureAsset Load(AssetLoadContext context,
                                 const std::string& asset_name)
        {
            const auto texture_content = context.file_reader->ReadBinaryFromFile(dir_name + "/" + asset_name);
            if (!texture_content.Ok())
            {
                throw std::runtime_error("Failed to load texture asset " + asset_name);
            }
            auto texture_asset = TextureAsset();
            Textures::TextureImporter::BuildTextureFromFile(texture_asset, texture_content.value.data, asset_name);
            return texture_asset;
        }
    };

    template <>
    struct AssetTraits<MaterialAsset>
    {
        using Handle = MaterialHandle;
        inline static const std::string dir_name = std::string("materials");

        static MaterialAsset Load(AssetLoadContext context,
                                  const std::string& asset_name)
        {
            const auto toml_file_content = context.file_reader->ReadTextFromFile(dir_name + "/" + asset_name);
            if (!toml_file_content.Ok())
            {
                throw std::runtime_error("Failed to load material asset " + asset_name);
            }
            Materials::MaterialFileData material_file_data{};
            Materials::MaterialImporter::ExtractMaterialFileData(material_file_data, toml_file_content.value);

            auto material = MaterialAsset();
            material.name = material_file_data.name;
            material.render_state = material_file_data.render_state;
            material.render_queue_index = material_file_data.render_queue_index;
            material.shader_handle = context.asset_handler->LoadShader(material_file_data.shader_name);
            material.base_color = material_file_data.base_color;
            material.albedo_texture.texture = context.asset_handler->
                                                      LoadTexture(material_file_data.albedo_texture.name);
            material.albedo_texture.tiling = material_file_data.albedo_texture.tiling;
            material.albedo_texture.uv_scale = material_file_data.albedo_texture.uv_scale;


            return material;
        }
    };

    template <>
    struct AssetTraits<InputMapAsset>
    {
        using Handle = InputMapHandle;
        inline static const std::string dir_name = std::string("inputmaps");

        static InputMapAsset Load(AssetLoadContext context, std::string asset_name)
        {
            const auto toml_file_content = context.file_reader->ReadTextFromFile(dir_name + "/" + asset_name);
            if (!toml_file_content.Ok())
            {
                throw std::runtime_error("Failed to load input map asset " + asset_name);
            }

            Input::InputMap input_map;
            InputMaps::InputMapImporter::ExtractInputMapFromFileData(input_map, toml_file_content.value);
            auto input_map_asset = InputMapAsset();
            input_map_asset.name = input_map.name;
            input_map_asset.input_map = input_map;
            return input_map_asset;
        }
    };
}
