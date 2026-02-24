//
// Created by sebastian on 22.02.26.
//

#pragma once

namespace Engine::Assets
{
    template <typename Tag>
    struct AssetId
    {
        std::size_t value{0};

        explicit constexpr operator bool() const noexcept
        {
            return value != 0;
        }

        friend bool operator==(const AssetId& a, const AssetId& b) noexcept
        {
            return a.value == b.value;
        }

        friend bool operator!=(const AssetId& a, const AssetId& b) noexcept
        {
            return a.value != b.value;
        }
    };

    struct ShaderTag
    {
    };

    struct TextureTag
    {
    };

    struct MeshTag
    {
    };

    struct FontTag
    {
    };

    struct MaterialTag
    {
    };

    using ShaderHandle = AssetId<ShaderTag>;
    using TextureHandle = AssetId<TextureTag>;
    using MeshHandle = AssetId<MeshTag>;
    using FontHandle = AssetId<FontTag>;
    using MaterialHandle = AssetId<MaterialTag>;
}

namespace std
{
    template <typename Tag>
    struct hash<Engine::Assets::AssetId<Tag>>
    {
        size_t operator()(const Engine::Assets::AssetId<Tag>& assetId) const noexcept
        {
            return std::hash<size_t>{}(assetId.value);
        }
    };
}
