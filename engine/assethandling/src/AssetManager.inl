#pragma once
#include "EnvironmentBuilder.hpp"
#include "AssetTypesImpl.hpp"

namespace Engine::AssetHandling
{
    inline AssetHandler::AssetHandler()
    {
        m_file_reader = Environment::EnvironmentBuilder::CreateFileManager();
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

    inline ShaderHandle AssetHandler::LoadShader(const std::string& shader_name)
    {
        return LoadAsset<ShaderAsset>(shader_name);
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

    inline void AssetHandler::UpdateMaterial(const MaterialHandle handle,
                                             const std::function<void(MaterialAsset&)>& updater)
    {
        UpdateAsset<MaterialAsset>(handle, updater);
    }

    inline void AssetHandler::UpdateTexture(const TextureHandle handle,
                                            const std::function<void(TextureAsset&)>& updater)
    {
        UpdateAsset<TextureAsset>(handle, updater);
    }

    inline void AssetHandler::UpdateMesh(const MeshHandle handle, const std::function<void(MeshAsset&)>& updater)
    {
        UpdateAsset<MeshAsset>(handle, updater);
    }

    template <AssetType T>
    AssetHandler::HandleT<T> AssetHandler::LoadAsset(const std::string& asset_name)
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
        cache.by_id.emplace(handle, AssetRecord<T>{
                                .asset = std::make_shared<T>(std::move(asset)),
                                .revision = 1,
                                .is_valid = true,
                            });

        return handle;
    }

    template <AssetType T>
    AssetHandler::HandleT<T> AssetHandler::RegisterAsset(T asset)
    {
        auto& cache = Cache<T>();
        typename AssetTraits<T>::Handle handle{cache.next_id++};
        cache.by_id.emplace(handle, AssetRecord<T>{
                                .asset = std::make_shared<T>(std::move(asset)),
                                .revision = 1,
                                .is_valid = true,
                            });
        return handle;
    }

    template <AssetType T>
    std::shared_ptr<const T> AssetHandler::GetAsset(HandleT<T> handle) const
    {
        auto& cache = Cache<T>();
        if (!handle)
        {
            throw std::invalid_argument("Handle is invalid!");
        }
        if (auto it = cache.by_id.find(handle); it != cache.by_id.end())
        {
            return it->second.asset;
        }

        throw std::invalid_argument("No asset found for handle");
    }

    template <AssetType T>
    uint32_t AssetHandler::GetAssetRevision(HandleT<T> handle) const
    {
        const auto& cache = Cache<T>();
        if (!handle)
        {
            throw std::invalid_argument("Handle is invalid!");
        }
        auto it = cache.by_id.find(handle);
        if (it == cache.by_id.end())
        {
            throw std::invalid_argument("No asset found for handle");
        }
        return it->second.revision;
    }

    template <AssetType T>
    T AssetHandler::LoadAssetWithoutCaching(const std::string& asset_name)
    {
        AssetLoadContext context{
            .file_reader = m_file_reader.get(),
            .asset_handler = this,
        };
        return AssetTraits<T>::Load(context, asset_name);
    }

    template <AssetType T>
    std::vector<AssetHandler::HandleT<T>> AssetHandler::GetAllAssetHandlesOfType()
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
        if (auto it = cache.id_by_name.find(asset_name); it != cache.id_by_name.end())
        {
            return it->second;
        }
        throw std::invalid_argument("No asset found for handle");
    }

    template <AssetType T>
    std::optional<AssetHandler::HandleT<T>> AssetHandler::FindAsset(const std::string& asset_name)
    {
        if (asset_name.empty())
        {
            return std::nullopt;
        }
        auto& cache = Cache<T>();
        if (auto it = cache.id_by_name.find(asset_name); it != cache.id_by_name.end())
        {
            return it->second;
        }
        return std::nullopt;
    }

    template <AssetType T, typename Fn>
    void AssetHandler::UpdateAsset(HandleT<T> handle, Fn&& updater)
    {
        auto& cache = Cache<T>();
        if (!handle)
        {
            throw std::invalid_argument("Handle is invalid!");
        }

        auto it = cache.by_id.find(handle);
        if (it == cache.by_id.end())
        {
            throw std::invalid_argument("No asset found for handle");
        }

        updater(*it->second.asset);
        ++it->second.revision;
    }
} // namespace
