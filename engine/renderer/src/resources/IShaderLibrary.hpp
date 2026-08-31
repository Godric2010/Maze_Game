//
// Created by sebastian on 22.03.26.
//

#pragma once
#include <vector>

#include "AssetTypes.hpp"
#include "Assets/AssetHandleTypes.hpp"

namespace yarep::Renderer::Resources
{
    class IShaderLibrary
    {
    public:
        virtual ~IShaderLibrary() = default;

        virtual void CompileShaders(
            const std::vector<std::tuple<assets::ShaderHandle, std::shared_ptr<const AssetHandling::ShaderAsset>>>& shaders) = 0;

        virtual void ClearShaders() = 0;
    };
}
