#if __APPLE__
   #include <catch2/catch_test_macros.hpp>
#else
    #include <catch2/catch_all.hpp>
#endif

#include "../include/IInputManager.hpp"

TEST_CASE("InputManagerTests - Set test name here") {
    REQUIRE(1 == 1);
}
