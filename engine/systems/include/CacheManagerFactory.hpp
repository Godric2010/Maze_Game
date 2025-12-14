#pragma once
#include <memory>

#include "ICacheManager.hpp"

namespace Engine::Systems {
    class CacheManagerFactory {
    public:
        static std::unique_ptr<ICacheManager> CreateCacheManager();
    };
} // namespace
