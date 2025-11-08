#include "SceneRegistry.hpp"

namespace Engine::Core {
    void SceneRegistry::RegisterScene(std::string name, SceneFactory factory) {
        m_registry[std::move(name)] = std::move(factory);
    }

    std::unique_ptr<IScene> SceneRegistry::CreateScene(const std::string &name, const SceneArgs &args) const {
        const auto it = m_registry.find(name);
        if (it != m_registry.end()) {
            return it->second(args);
        }
        return nullptr;
    }
} // namespace
