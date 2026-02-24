//
// Created by sebastian on 01.02.26.
//

#pragma once
#include <string>

#include "Assets/AssetHandleTypes.hpp"

namespace Engine::Renderer
{
    class IRenderer
    {
    public:
        virtual ~IRenderer() = default;

        virtual Assets::MeshHandle GetOrLoadMesh(const std::string& file_path) = 0;

        virtual Assets::TextureHandle GetOrLoadTexture(const std::string& file_path) = 0;

        virtual Assets::MaterialHandle GetOrLoadMaterial(const std::string& file_path) = 0;
    };
}
