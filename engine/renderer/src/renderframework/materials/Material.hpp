//
// Created by sebastian on 24.02.26.
//

#pragma once
#include "AssetTypes.hpp"
#include "Assets/AssetHandleTypes.hpp"

namespace Engine::Renderer::RenderFramework::Materials
{
    struct MaterialTextureRef
    {
        Assets::TextureHandle texture;
        glm::vec2 uv_scale;
        glm::vec2 tiling;
    };

    struct Material
    {
        AssetHandling::RenderState render_state;
        Assets::ShaderHandle shader;
        MaterialTextureRef albedo_texture;
        glm::vec4 base_color;
    };
}
