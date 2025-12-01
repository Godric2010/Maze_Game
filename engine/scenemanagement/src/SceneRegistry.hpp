#pragma once
#include <functional>
#include <memory>
#include <unordered_map>
#include "../include/IScene.hpp"
#include "Scene/SceneArgs.hpp"

namespace Engine::SceneManagement {
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
