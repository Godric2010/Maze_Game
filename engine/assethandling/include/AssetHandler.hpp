#pragma once
#include <IFileReader.hpp>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>
#include "AssetTypes.hpp"
#include "Assets/IAssetLibrary.hpp"

namespace Engine::AssetHandling
{
    template <typename T>
    struct AssetTraits;

    struct AssetLoadContext;

    class AssetHandler : public Assets::IAssetLibrary
    {
    public:
        AssetHandler();

        ~AssetHandler() override = default;

        Assets::MeshHandle LoadMesh(const std::string& mesh_name) override;
        Assets::MaterialHandle LoadMaterial(const std::string& name) override;
        Assets::TextureHandle LoadTexture(const std::string& texture_name) override;

        std::optional<Assets::MeshHandle> FindMesh(const std::string& mesh_name) override;
        std::optional<Assets::MaterialHandle> FindMaterial(const std::string& material_name) override;
        std::optional<Assets::TextureHandle> FindTexture(const std::string& texture_name) override;
        std::optional<Assets::FontHandle> FindFont(const std::string& font_name) override;
        std::optional<Assets::ShaderHandle> FindShader(const std::string& shader_name) override;

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
        template <AssetType T>
        std::optional<HandleT<T>> FindAsset(const std::string& asset_name);
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

    struct AssetLoadContext
    {
        Environment::Files::IFileReader* file_reader;
        AssetHandler* asset_handler;
    };
} // namespace
#include "../src/AssetManager.inl"
