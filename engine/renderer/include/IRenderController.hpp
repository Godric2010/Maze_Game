//
// Created by Sebastian Borsch on 14.12.25.
//

#pragma once
#include "Datatypes.hpp"

namespace Engine::Renderer {
    class IRenderController {
    public:
        virtual ~IRenderController() = default;

        [[nodiscard]] virtual MeshHandle RegisterMesh(const MeshAsset& mesh) const =0;

        virtual void UnregisterMesh(const MeshHandle& handle) const = 0;

        [[nodiscard]] virtual TextureHandle RegisterTexture(const TextureAsset& texture) const = 0;

        virtual void UnregisterTexture(const TextureHandle& handle) const = 0;

        virtual void BeginFrame(const CameraAsset& camera_asset) const = 0;

        virtual void SubmitFrame(DrawAssets& draw_assets) const = 0;

        virtual void SubmitDebugInfos(const std::vector<UiDrawAsset>& debug_draw_assets) = 0;

        [[nodiscard]] virtual uint32_t GetDrawCalls() const = 0;
    };
}
