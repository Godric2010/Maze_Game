//
// Created by sebastian on 23.08.26.
//

#pragma once
#include <glm/vec4.hpp>

namespace Engine::Renderer::RenderFramework::OpenGL {
    constexpr int MAX_POINT_LIGHTS = 64;

    struct alignas(16) PointLightAsset {
        glm::vec4 position;
        glm::vec4 color_intensity;
    };

    static_assert(sizeof(PointLightAsset) == 32);

    struct alignas(16) GpuLightingData {
        glm::vec4 ambient_color_intensity;
        glm::ivec4 light_meta;
        PointLightAsset point_light[MAX_POINT_LIGHTS];
    };
}
