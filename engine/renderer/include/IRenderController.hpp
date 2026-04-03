//
// Created by Sebastian Borsch on 14.12.25.
//

#pragma once
#include "Datatypes.hpp"
#include <memory>
#include "AssetTypes.hpp"

namespace Engine::Renderer
{
    class IRenderController 
    {
    public:
        virtual ~IRenderController() = default;

        virtual void SubmitDebugInfos(const std::vector<DrawAsset>& debug_draw_assets) = 0;

        virtual void RenderFrame(const CameraAsset& camera_asset, std::vector<DrawAsset> draw_assets) const = 0;

        virtual Assets::MeshHandle GetUIMeshHandle() const = 0;

        [[nodiscard]] virtual uint32_t GetDrawCalls() const = 0;
    };
}
