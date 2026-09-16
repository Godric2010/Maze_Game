//
// Created by sebastian on 24.02.26.
//

#pragma once
#include <Math.hpp>
#include "AssetTypes.hpp"
#include "Assets/AssetHandleTypes.hpp"

namespace yarep::renderer::render_framework::open_gl {
    struct MaterialTextureRef {
        assets::TextureHandle texture;
        math::Vec2 uv_scale;
        math::Vec2 tiling;
    };

    struct OpenGlMaterial {
        asset_handling::RenderState render_state;
        assets::ShaderHandle shader;
        MaterialTextureRef albedo_texture;
        math::Vec4 base_color;
    };
}
