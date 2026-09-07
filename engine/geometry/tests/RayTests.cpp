#if __APPLE__
    #include <catch2/catch_test_macros.hpp>
    #include <utility>
#else
    #include <catch2/catch_all.hpp>
#endif

#include "../include/Ray.hpp"

using namespace yarep::geometry;
using namespace yarep::math;


// ============================================================================
// construction
// ============================================================================

TEST_CASE("Ray default constructor creates zero ray") {
    constexpr Ray ray{};

    REQUIRE(ray.origin == Vec3{0, 0, 0});
    REQUIRE(ray.direction == Vec3{0, 0, 0});
}


TEST_CASE("Ray constructor stores origin and direction") {
    constexpr Ray ray{
        Vec3{1, 2, 3},
        Vec3{0, 0, -1}
    };

    REQUIRE(ray.origin == Vec3{1, 2, 3});
    REQUIRE(ray.direction == Vec3{0, 0, -1});
}


TEST_CASE("Ray constructor supports negative coordinates") {
    constexpr Ray ray{
        Vec3{-10, -20, -30},
        Vec3{-1, 0, 0}
    };

    REQUIRE(ray.origin == Vec3{-10, -20, -30});
    REQUIRE(ray.direction == Vec3{-1, 0, 0});
}


TEST_CASE("Ray constructor supports arbitrary normalized direction") {
    constexpr Ray ray{
        Vec3{4, -2, 7},
        Vec3{0.6f, 0.0f, 0.8f}
    };

    REQUIRE(ray.origin == Vec3{4, -2, 7});
    REQUIRE(ray.direction == Vec3{0.6f, 0.0f, 0.8f});
}


// ============================================================================
// equality
// ============================================================================

TEST_CASE("Ray equality recognizes identical rays") {
    constexpr Ray lhs{
        Vec3{1, 2, 3},
        Vec3{0, 0, -1}
    };

    constexpr Ray rhs{
        Vec3{1, 2, 3},
        Vec3{0, 0, -1}
    };

    REQUIRE(lhs == rhs);
}


TEST_CASE("Ray equality rejects different origin") {
    constexpr Ray lhs{
        Vec3{1, 2, 3},
        Vec3{0, 0, -1}
    };

    constexpr Ray rhs{
        Vec3{2, 2, 3},
        Vec3{0, 0, -1}
    };

    REQUIRE_FALSE(lhs == rhs);
}


TEST_CASE("Ray equality rejects different direction") {
    constexpr Ray lhs{
        Vec3{1, 2, 3},
        Vec3{0, 0, -1}
    };

    constexpr Ray rhs{
        Vec3{1, 2, 3},
        Vec3{1, 0, 0}
    };

    REQUIRE_FALSE(lhs == rhs);
}


TEST_CASE("Ray inequality recognizes different rays") {
    constexpr Ray lhs{
        Vec3{1, 2, 3},
        Vec3{0, 0, -1}
    };

    constexpr Ray rhs{
        Vec3{1, 2, 3},
        Vec3{1, 0, 0}
    };

    REQUIRE(lhs != rhs);
}


// ============================================================================
// value semantics
// ============================================================================

TEST_CASE("Ray copy preserves values") {
    constexpr Ray original{
        Vec3{-5, 4, 3},
        Vec3{0, 1, 0}
    };

    constexpr Ray copy = original;

    REQUIRE(copy == original);
}


// ============================================================================
// constexpr contract
// ============================================================================

static_assert(
    Ray{}.origin
    == Vec3{0, 0, 0});

static_assert(
    Ray{}.direction
    == Vec3{0, 0, 0});

static_assert(
    Ray{
        Vec3{1, 2, 3},
        Vec3{0, 0, -1}
    }.origin
    == Vec3{1, 2, 3});

static_assert(
    Ray{
        Vec3{1, 2, 3},
        Vec3{0, 0, -1}
    }.direction
    == Vec3{0, 0, -1});

static_assert(
    Ray{
        Vec3{1, 2, 3},
        Vec3{0, 0, -1}
    }
    ==
    Ray{
        Vec3{1, 2, 3},
        Vec3{0, 0, -1}
    });