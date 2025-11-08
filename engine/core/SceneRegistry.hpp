#pragma once
#include <unordered_map>
#include <functional>
#include <memory>
#include <any>
#include "IScene.hpp"

namespace Engine::Core {
    struct SceneArgs {
        std::any payload;
    };

    using SceneFactory = std::function<std::unique_ptr<IScene>(SceneArgs) >;

    class SceneRegistry {
    public:
        SceneRegistry() = default;

        ~SceneRegistry() = default;

        void RegisterScene(std::string name, SceneFactory factory);

        [[nodiscard]] std::unique_ptr<IScene> CreateScene(const std::string &name, const SceneArgs &args) const;

    private:
        std::unordered_map<std::string, SceneFactory> m_registry;
    };
} // namespace
