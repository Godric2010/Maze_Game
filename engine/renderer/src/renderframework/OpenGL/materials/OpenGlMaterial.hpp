//
// Created by sebastian on 24.02.26.
//

#pragma once
#include "AssetTypes.hpp"
#include "Assets/AssetHandleTypes.hpp"

namespace yarep::Renderer::RenderFramework::OpenGl
{
    struct MaterialTextureRef
    {
        assets::TextureHandle texture;
        glm::vec2 uv_scale;
        glm::vec2 tiling;
    };

    struct OpenGlMaterial
    {
        AssetHandling::RenderState render_state;
        assets::ShaderHandle shader;
        MaterialTextureRef albedo_texture;
        glm::vec4 base_color;
    };
}
