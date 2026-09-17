//
// Created by Sebastian Borsch on 14.08.25.
//

#pragma once
#include <vector>

#include "AssetTypes.hpp"
#include "Assets/AssetHandleTypes.hpp"
#include "Ecs/Types.hpp"

namespace yarep::renderer {
    struct CameraAsset {
        math::Mat4 view;
        math::Mat4 projection;
        math::Vec4 camera_position;
    };

    struct AmbientLightAsset {
        math::Vec3 color;
        float intensity;
    };

    struct LightAsset {
        math::Vec3 position;
        math::Vec3 color;
        float intensity;
        float constant_attenuation;
        float linear_attenuation;
        float quadratic_attenuation;
    };

    struct FrameData {
        CameraAsset camera;
        AmbientLightAsset ambient_light;
        std::vector<LightAsset> lights;
    };

    struct DrawAsset {
        ecs::EntityId entity;
        asset_handling::RenderState render_state;
        size_t render_queue_index;
        assets::MeshHandle mesh;
        assets::MaterialHandle material;
        math::Mat4 model;
        math::Vec4 color;
    };
}
