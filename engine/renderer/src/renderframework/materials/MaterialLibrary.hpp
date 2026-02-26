//
// Created by sebastian on 24.02.26.
//

#pragma once
#include "AssetHandler.hpp"
#include "Material.hpp"

namespace Engine::Renderer::RenderFramework::Materials
{
    class MaterialLibrary
    {
    public:
        MaterialLibrary(AssetHandling::AssetHandler* asset_handler);

        ~MaterialLibrary();

        void Add(Assets::MaterialHandle material_handle);

        void Remove(Assets::MaterialHandle material);

        Material& Get(Assets::MaterialHandle material);

        void Clear();

    private:
        AssetHandling::AssetHandler* m_asset_handler;
        std::unordered_map<Assets::MaterialHandle, Material> m_material_map;
    };
}
