#if __APPLE__
   #include <catch2/catch_test_macros.hpp>
   #include <utility>  
#else
    #include <catch2/catch_all.hpp>
#endif

#include "../include/AABB.hpp"

TEST_CASE("AABBTests - Set test name here"){
    
    REQUIRE(1 == 1);
} 