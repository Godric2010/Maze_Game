//
// Created by sebastian on 24.02.26.
//

#pragma once
#include "AssetTypes.hpp"
#include "Assets/AssetHandleTypes.hpp"

namespace yarep::renderer::render_framework::open_gl
{
    struct MaterialTextureRef
    {
        assets::TextureHandle texture;
        glm::vec2 uv_scale;
        glm::vec2 tiling;
    };

    struct OpenGlMaterial
    {
        asset_handling::RenderState render_state;
        assets::ShaderHandle shader;
        MaterialTextureRef albedo_texture;
        glm::vec4 base_color;
    };
}
