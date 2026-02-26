#pragma once
#include <IFileReader.hpp>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>
#include "AssetTypes.hpp"

namespace Engine::AssetHandling
{
    template <typename T>
    struct AssetTraits;

    class AssetHandler
    {
    public:
        AssetHandler();

        ~AssetHandler() = default;

        template <AssetType T>
        using HandleT = typename AssetTraits<T>::Handle;
        
        template <AssetType T>
        HandleT<T> LoadAsset(const std::string& asset_name);
        
        template <AssetType T>
        HandleT<T> RegisterAsset(std::shared_ptr<T> asset);
        
        template <AssetType T>
        std::shared_ptr<T> GetAsset(HandleT<T> handle) const;

        template <AssetType T>
        std::shared_ptr<T> LoadAssetWithoutCaching(const std::string& asset_name);

        template <AssetType T>
        std::vector<HandleT<T>> GetAllAssetHandlesOfType();
        
        template <AssetType T>
        HandleT<T> GetHandleFromName(const std::string& asset_name);

    private:
        std::unique_ptr<Environment::Files::IFileReader> m_file_reader;

        template <typename T>
        struct AssetCache
        {
            using Handle = typename AssetTraits<T>::Handle;
            
            std::unordered_map<Handle, std::shared_ptr<T>> by_id;
            std::unordered_map<std::string, Handle> id_by_name;
            size_t next_id = 1;
        };

        template <AssetType T>
        static AssetCache<T>& Cache()
        {
            static AssetCache<T> cache;
            return cache;
        }
    };
} // namespace
#include "../src/AssetManager.inl"
