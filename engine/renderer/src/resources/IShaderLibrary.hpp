//
// Created by sebastian on 22.03.26.
//

#pragma once
#include <vector>

#include "AssetTypes.hpp"
#include "Assets/AssetHandleTypes.hpp"

namespace yarep::renderer::resources
{
    class IShaderLibrary
    {
    public:
        virtual ~IShaderLibrary() = default;

        virtual void CompileShaders(
            const std::vector<std::tuple<assets::ShaderHandle, std::shared_ptr<const asset_handling::ShaderAsset>>>& shaders) = 0;

        virtual void ClearShaders() = 0;
    };
}
