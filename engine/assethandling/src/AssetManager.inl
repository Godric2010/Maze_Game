#pragma once
#include "EnvironmentBuilder.hpp"
#include "AssetTypesImpl.hpp"

namespace Engine::AssetHandling
{
    inline AssetHandler::AssetHandler()
    {
        m_file_reader = Environment::EnvironmentBuilder::CreateFileReader();
    }

    inline MeshHandle AssetHandler::LoadMesh(const std::string& mesh_name)
    {
        return LoadAsset<MeshAsset>(mesh_name);
    }

    inline MaterialHandle AssetHandler::LoadMaterial(const std::string& name)
    {
        return LoadAsset<MaterialAsset>(name);
    }

    inline TextureHandle AssetHandler::LoadTexture(const std::string& texture_name)
    {
        return LoadAsset<TextureAsset>(texture_name);
    }

    inline std::optional<MeshHandle> AssetHandler::FindMesh(const std::string& mesh_name)
    {
        return FindAsset<MeshAsset>(mesh_name);
    }

    inline std::optional<MaterialHandle> AssetHandler::FindMaterial(const std::string& material_name)
    {
        return FindAsset<MaterialAsset>(material_name);
    }

    inline std::optional<TextureHandle> AssetHandler::FindTexture(const std::string& texture_name)
    {
        return FindAsset<TextureAsset>(texture_name);
    }

    inline std::optional<FontHandle> AssetHandler::FindFont(const std::string& font_name)
    {
        return FindAsset<FontAsset>(font_name);
    }

    inline std::optional<ShaderHandle> AssetHandler::FindShader(const std::string& shader_name)
    {
        return FindAsset<ShaderAsset>(shader_name);
    }

    template <AssetType T>
    typename AssetHandler::HandleT<T> AssetHandler::LoadAsset(const std::string& asset_name)
    {
        if (asset_name.empty())
        {
            typename AssetTraits<T>::Handle handle{0};
            return handle;
        }

        auto& cache = Cache<T>();

        if (auto it = cache.id_by_name.find(asset_name); it != cache.id_by_name.end())
        {
            return it->second;
        }

        AssetLoadContext context{
            .file_reader = m_file_reader.get(),
            .asset_handler = this,
        };

        auto asset = AssetTraits<T>::Load(context, asset_name);

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
        AssetLoadContext context{
            .file_reader = m_file_reader.get(),
            .asset_handler = this,
        };
        return AssetTraits<T>::Load(context, asset_name);
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

    template <AssetType T>
    std::optional<AssetHandler::HandleT<T>> AssetHandler::FindAsset(const std::string& asset_name)
    {
        if (asset_name.empty())
        {
            return std::nullopt;
        }
        auto& cache = Cache<T>();
        if (cache.id_by_name.find(asset_name) != cache.id_by_name.end())
        {
            return cache.id_by_name[asset_name];
        }
        return std::nullopt;
    }
} // namespace
