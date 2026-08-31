//
// Created by Sebastian Borsch on 24.08.25.
//

#pragma once
#include <Input/IInput.hpp>
#include "IServiceToEcsProvider.hpp"
#include "World.hpp"

#include "../../systems/src/CacheManager.hpp"
#include "Ecs/ISystem.hpp"

namespace yarep::ecs {

    class IEngineSystem : public ISystem {
        friend class SystemManager;

    public:
        ~IEngineSystem() override = default;

    private:
        IServiceToEcsProvider* m_service_locator{};
        World* m_world{};
        systems::CacheManager* m_cache_manager{};

    protected:
        [[nodiscard]] IServiceToEcsProvider* ServiceLocator() const { return m_service_locator; }
        [[nodiscard]] World* EcsWorld() const { return m_world; }
        [[nodiscard]] systems::CacheManager* Cache() const { return m_cache_manager; }
    };
}
