#pragma once
#include "EnvironmentBuilder.hpp"
#include "AssetTypesImpl.hpp"

namespace Engine::AssetHandling {
    inline AssetHandler::AssetHandler() {
        m_file_reader = Environment::EnvironmentBuilder::CreateFileReader();
    }

    template<AssetType T>
    std::shared_ptr<T> AssetHandler::LoadAsset(const std::string& asset_name) {
        auto& cache = Cache<T>();

        if (auto it = cache.find(asset_name); it != cache.end()) {
            return it->second;
        }

        auto asset = AssetTraits<T>::Load(m_file_reader.get(), asset_name);
        cache.emplace(asset_name, asset);
        return asset;
    }

    template<AssetType T>
    std::vector<std::shared_ptr<T> > AssetHandler::GetAllAssetsOfType() {
        auto& cache = Cache<T>();
        std::vector<std::shared_ptr<T> > result;
        for (auto& it: cache) {
            result.push_back(it.second);
        }
        return result;
    }
} // namespace
