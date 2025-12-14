#include "CacheManagerFactory.hpp"
#include "CacheManager.hpp"

namespace Engine::Systems {
    std::unique_ptr<ICacheManager> CacheManagerFactory::CreateCacheManager() {
        return std::make_unique<CacheManager>();
    }
} // namespace
