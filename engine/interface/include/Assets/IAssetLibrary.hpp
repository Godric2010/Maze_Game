//
// Created by sebastian on 22.03.26.
//

#pragma once
#include <optional>
#include <string>

#include "AssetHandleTypes.hpp"

namespace Engine::Assets
{
    class IAssetLibrary
    {
    public:
        virtual ~IAssetLibrary() = default;

        virtual MeshHandle LoadMesh(const std::string& name) = 0;
        virtual MaterialHandle LoadMaterial(const std::string& name) = 0;
        virtual TextureHandle LoadTexture(const std::string& name) = 0;

        virtual std::optional<MeshHandle> FindMesh(const std::string& name) = 0;
        virtual std::optional<MaterialHandle> FindMaterial(const std::string& name) = 0;
        virtual std::optional<TextureHandle> FindTexture(const std::string& name) = 0;
        virtual std::optional<ShaderHandle> FindShader(const std::string& name) = 0;
        virtual std::optional<FontHandle> FindFont(const std::string& name) = 0;

    private:
    };
}
