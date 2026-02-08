//
// Created by sebastian on 01.02.26.
//

#pragma once
#include <string>

#include "Types.hpp"

namespace Engine::Renderer
{
    class IRenderer
    {
    public:
        virtual ~IRenderer() = default;
        
        virtual MeshHandle GetOrLoadMesh(const std::string& file_path) = 0;
        
        virtual TextureHandle GetOrLoadTexture(const std::string& file_path) = 0;
    };
}
