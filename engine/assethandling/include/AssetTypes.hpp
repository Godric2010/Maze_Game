#pragma once
#include <string>

namespace Engine::AssetHandling {
    struct Asset {
        virtual ~Asset() = default;
    };

    template<typename T>
    concept AssetType = std::is_base_of_v<Asset, T>;

    struct ShaderAsset final : Asset {
        std::string name;
        std::string vertex_content;
        std::string fragment_content;
    };
}
