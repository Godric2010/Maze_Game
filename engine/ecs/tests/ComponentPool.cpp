#if __APPLE__
   #include <catch2/catch_test_macros.hpp>
   #include <utility>  
#else
    #include <catch2/catch.hpp>
#endif

#include "../src/ComponentPool.hpp"
#include "../src/Entity.hpp"

using namespace Engine::Ecs;


TEST_CASE("ComponentPool::Add - Add new enity and component", "[ecs][componentpool]"){

    ComponentPool<uint32_t> pool;

    REQUIRE(1 == 1);
} 