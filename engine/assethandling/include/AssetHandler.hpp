#pragma once
#include <IFileManager.hpp>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>
#include <functional>
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
            Assets::ShaderHandle LoadShader(const std::string& shader_name);

            std::optional<Assets::MeshHandle> FindMesh(const std::string& mesh_name) override;
            std::optional<Assets::MaterialHandle> FindMaterial(const std::string& material_name) override;
            std::optional<Assets::TextureHandle> FindTexture(const std::string& texture_name) override;
            std::optional<Assets::FontHandle> FindFont(const std::string& font_name) override;
            std::optional<Assets::ShaderHandle> FindShader(const std::string& shader_name) override;

            void UpdateMaterial(Assets::MaterialHandle handle, const std::function<void(MaterialAsset&)>& updater);
            void UpdateTexture(Assets::TextureHandle handle, const std::function<void(TextureAsset&)>& updater);
            void UpdateMesh(Assets::MeshHandle handle, const std::function<void(MeshAsset&)>& updater);

            template <AssetType T>
            using HandleT = typename AssetTraits<T>::Handle;

            template <AssetType T>
            HandleT<T> LoadAsset(const std::string& asset_name);

            template <AssetType T>
            HandleT<T> RegisterAsset(T asset);

            template <AssetType T, typename Fn>
            void UpdateAsset(HandleT<T> handle, Fn&& updater);

            template <AssetType T>
            std::optional<HandleT<T>> FindAsset(const std::string& asset_name);

            template <AssetType T>
            std::shared_ptr<const T> GetAsset(HandleT<T> handle) const;

            template <AssetType T>
            uint32_t GetAssetRevision(HandleT<T> handle) const;

            template <AssetType T>
            T LoadAssetWithoutCaching(const std::string& asset_name);

            template <AssetType T>
            std::vector<HandleT<T>> GetAllAssetHandlesOfType();

            template <AssetType T>
            HandleT<T> GetHandleFromName(const std::string& asset_name);

            template <AssetType T>
            static void ClearCache()
            {
                Cache<T>() = {};
            }

        private:
            std::unique_ptr<Environment::Files::IFileManager> m_file_reader;

            template <AssetType T>
            struct AssetRecord
            {
                std::shared_ptr<T> asset;
                uint32_t revision = 1;
                bool is_valid = true;
            };

            template <typename T>
            struct AssetCache
            {
                using Handle = typename AssetTraits<T>::Handle;

                std::unordered_map<Handle, AssetRecord<T>> by_id;
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
        Environment::Files::IFileManager* file_reader;
        AssetHandler* asset_handler;
    };
} // namespace
#include "../src/AssetManager.inl"
