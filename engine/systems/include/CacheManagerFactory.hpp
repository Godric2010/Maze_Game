#pragma once
#include <memory>

#include "ICacheManager.hpp"

namespace yarep::Systems {
    class CacheManagerFactory {
    public:
        static std::unique_ptr<ICacheManager> CreateCacheManager();
    };
} // namespace
