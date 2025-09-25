#include "../include/Types.hpp"
#if __APPLE__
#include <catch2/catch_test_macros.hpp>
#include <utility>
#else
#include <catch2/catch_all.hpp>
#endif

using namespace Engine::Physics;

TEST_CASE("Physics", "[Physics]") {
    REQUIRE(1 == 1);
}