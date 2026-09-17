//
// Created by sebastian on 23.08.26.
//

#pragma once

namespace yarep::renderer::render_framework::open_gl {
    constexpr int max_point_lights = 64;

    struct alignas(16) PointLightAsset {
        math::Vec4 position;
        math::Vec4 color_intensity;
        math::Vec4 attenuation;
    };

    static_assert(sizeof(PointLightAsset) == 48);

    struct alignas(16) GpuIVec4 {
        std::int32_t x;
        std::int32_t y;
        std::int32_t z;
        std::int32_t w;
    };

    struct alignas(16) GpuLightingData {
        math::Vec4 ambient_color_intensity;
        GpuIVec4 light_meta;
        PointLightAsset point_light[max_point_lights];
    };
}
