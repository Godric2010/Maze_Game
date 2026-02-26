#pragma once
#include "EnvironmentBuilder.hpp"
#include "AssetTypesImpl.hpp"

namespace Engine::AssetHandling
{
    inline AssetHandler::AssetHandler()
    {
        m_file_reader = Environment::EnvironmentBuilder::CreateFileReader();
    }

    template <AssetType T>
    typename AssetHandler::HandleT<T> AssetHandler::LoadAsset(const std::string& asset_name)
    {
        auto& cache = Cache<T>();

        if (auto it = cache.id_by_name.find(asset_name); it != cache.id_by_name.end())
        {
            return it->second;
        }

        auto asset = AssetTraits<T>::Load(m_file_reader.get(), asset_name);

        typename AssetTraits<T>::Handle handle{cache.next_id++};
        cache.id_by_name.emplace(asset_name, handle);
        cache.by_id.emplace(handle, std::move(asset));

        return handle;
    }

    template <AssetType T>
    AssetHandler::HandleT<T> AssetHandler::RegisterAsset(std::shared_ptr<T> asset)
    {
        auto& cache = Cache<T>();
        typename AssetTraits<T>::Handle handle{cache.next_id++};
        cache.by_id.emplace(handle, std::move(asset));
        return handle;
    }

    template <AssetType T>
    std::shared_ptr<T> AssetHandler::GetAsset(HandleT<T> handle) const
    {
        auto& cache = Cache<T>();
        if (!handle)
        {
            throw std::invalid_argument("Handle is invalid!");
        }
        if (auto it = cache.by_id.find(handle); it != cache.by_id.end())
        {
            return it->second;
        }

        throw std::invalid_argument("No asset found for handle");
    }

    template <AssetType T>
    std::shared_ptr<T> AssetHandler::LoadAssetWithoutCaching(const std::string& asset_name)
    {
        return AssetTraits<T>::Load(m_file_reader.get(), asset_name);
    }

    template <AssetType T>
    std::vector<typename AssetHandler::HandleT<T>> AssetHandler::GetAllAssetHandlesOfType()
    {
        auto& cache = Cache<T>();
        std::vector<HandleT<T>> result;
        result.reserve(cache.id_by_name.size());

        for (auto& [id, _] : cache.by_id)
        {
            result.push_back(HandleT<T>{id});
        }

        return result;
    }

    template <AssetType T>
    AssetHandler::HandleT<T> AssetHandler::GetHandleFromName(const std::string& asset_name)
    {
        auto& cache = Cache<T>();
        return cache.id_by_name[asset_name];
    }
} // namespace
