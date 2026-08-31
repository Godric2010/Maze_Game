//
// Created by sebastian on 22.02.26.
//

#pragma once

namespace yarep::assets
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

    struct InputMapTag
    {
    };

    using ShaderHandle = AssetId<ShaderTag>;
    using TextureHandle = AssetId<TextureTag>;
    using MeshHandle = AssetId<MeshTag>;
    using FontHandle = AssetId<FontTag>;
    using MaterialHandle = AssetId<MaterialTag>;
    using InputMapHandle = AssetId<InputMapTag>;
}

namespace std
{
    template <typename Tag>
    struct hash<yarep::assets::AssetId<Tag>>
    {
        size_t operator()(const yarep::assets::AssetId<Tag>& asset_id) const noexcept
        {
            return std::hash<size_t>{}(asset_id.value);
        }
    };
}
