#pragma once
#include <string>
#include <vector>
#include <glm/glm.hpp>

#include "Assets/AssetHandleTypes.hpp"
#include "Input/InputTypes.hpp"

namespace Engine::AssetHandling
{
    struct Asset
    {
        virtual ~Asset() = default;
    };

    template <typename T>
    concept AssetType = std::is_base_of_v<Asset, T>;

    struct ShaderAsset final : Asset
    {
        std::string name;
        std::string vertex_content;
        std::string fragment_content;
    };

    struct FontAsset : Asset
    {
        std::string name;
        std::vector<uint8_t> bytes;
    };

    struct MeshVertexAsset
    {
        glm::vec3 position;
        glm::vec3 normal;
        glm::vec2 uv;
    };

    struct MeshAsset : Asset
    {
        std::vector<MeshVertexAsset> vertices;
        std::vector<uint32_t> indices;

        bool IsValid() const
        {
            return vertices.size() >= 3 && indices.size() >= 3;
        }
    };

    enum class PixelFormat
    {
        R8,
        RGB8,
        RGBA8,
    };

    struct TextureAsset : Asset
    {
        uint32_t width;
        uint32_t height;
        PixelFormat format;
        std::vector<uint8_t> pixels;
    };

    #define RenderStateList \
        X(Opaque) \
        X(UI)

    enum class RenderState
    {
        #define X(name) name,
        RenderStateList
        #undef X
    };

    constexpr std::array<std::pair<std::string_view, RenderState>, 2> RenderStateMap = {
        {
            #define X(name) {#name, RenderState::name},
            RenderStateList
            #undef X
        }
    };

    struct MaterialTexture
    {
        Assets::TextureHandle texture;
        glm::vec2 uv_scale;
        glm::vec2 tiling;
    };

    struct MaterialAsset : Asset
    {
        std::string name;
        RenderState render_state;
        size_t render_queue_index;
        Assets::ShaderHandle shader_handle;
        MaterialTexture albedo_texture;
        glm::vec4 base_color;
    };

    struct InputMapAsset : Asset
    {
        std::string name;
        Input::InputMap input_map;
    };
}
