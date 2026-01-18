#pragma once
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>
#include "AssetTypes.hpp"
#include "IFileReader.hpp"

namespace Engine::AssetHandling {
    class AssetHandler {
    public:
        AssetHandler();

        ~AssetHandler() = default;

        template<AssetType T>
        std::shared_ptr<T> LoadAsset(const std::string& asset_name);

        template<AssetType T>
        std::vector<std::shared_ptr<T> > GetAllAssetsOfType();

    private:
        std::unique_ptr<Environment::Files::IFileReader> m_file_reader;

        template<AssetType T>
        static std::unordered_map<std::string, std::shared_ptr<T> > &Cache() {
            static std::unordered_map<std::string, std::shared_ptr<T> > cache;
            return cache;
        }
    };
} // namespace
#include "../src/AssetManager.inl"
