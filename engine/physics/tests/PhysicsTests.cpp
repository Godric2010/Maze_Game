#include "../include/math/Types.hpp"
#if __APPLE__
#include <catch2/catch_test_macros.hpp>
#include <utility>
#else
#include <catch2/catch_all.hpp>
#endif

using namespace yarep::physics;

TEST_CASE("Physics", "[Physics]") {
    REQUIRE(1 == 1);
}