#if __APPLE__
    #include <catch2/catch_test_macros.hpp>
    #include <utility>
#else
    #include <catch2/catch_all.hpp>
#endif

#include "../include/Sphere.hpp"

using namespace yarep::geometry;
using namespace yarep::math;


// ============================================================================
// construction
// ============================================================================

TEST_CASE("Sphere default constructor creates zero radius sphere at origin") {
    constexpr Sphere sphere{};

    REQUIRE(sphere.center == Vec3{0, 0, 0});
    REQUIRE(sphere.radius == 0.0f);
}


TEST_CASE("Sphere constructor stores center and radius") {
    constexpr Sphere sphere{
        Vec3{1, 2, 3},
        5.0f
    };

    REQUIRE(sphere.center == Vec3{1, 2, 3});
    REQUIRE(sphere.radius == 5.0f);
}


TEST_CASE("Sphere constructor supports negative center coordinates") {
    constexpr Sphere sphere{
        Vec3{-10, -20, -30},
        4.0f
    };

    REQUIRE(sphere.center == Vec3{-10, -20, -30});
    REQUIRE(sphere.radius == 4.0f);
}


TEST_CASE("Sphere supports zero radius") {
    constexpr Sphere sphere{
        Vec3{2, -4, 7},
        0.0f
    };

    REQUIRE(sphere.center == Vec3{2, -4, 7});
    REQUIRE(sphere.radius == 0.0f);
}


// ============================================================================
// equality
// ============================================================================

TEST_CASE("Sphere equality recognizes identical spheres") {
    constexpr Sphere lhs{
        Vec3{1, 2, 3},
        5.0f
    };

    constexpr Sphere rhs{
        Vec3{1, 2, 3},
        5.0f
    };

    REQUIRE(lhs == rhs);
}


TEST_CASE("Sphere equality rejects different center") {
    constexpr Sphere lhs{
        Vec3{1, 2, 3},
        5.0f
    };

    constexpr Sphere rhs{
        Vec3{1, 2, 4},
        5.0f
    };

    REQUIRE_FALSE(lhs == rhs);
}


TEST_CASE("Sphere equality rejects different radius") {
    constexpr Sphere lhs{
        Vec3{1, 2, 3},
        5.0f
    };

    constexpr Sphere rhs{
        Vec3{1, 2, 3},
        6.0f
    };

    REQUIRE_FALSE(lhs == rhs);
}


TEST_CASE("Sphere inequality recognizes different spheres") {
    constexpr Sphere lhs{
        Vec3{1, 2, 3},
        5.0f
    };

    constexpr Sphere rhs{
        Vec3{1, 2, 3},
        6.0f
    };

    REQUIRE(lhs != rhs);
}


// ============================================================================
// value semantics
// ============================================================================

TEST_CASE("Sphere copy preserves values") {
    constexpr Sphere original{
        Vec3{-5, 4, 3},
        7.0f
    };

    constexpr Sphere copy = original;

    REQUIRE(copy == original);
}


// ============================================================================
// constexpr contract
// ============================================================================

static_assert(
    Sphere{}.center
    == Vec3{0, 0, 0});

static_assert(
    Sphere{}.radius
    == 0.0f);

static_assert(
    Sphere{
        Vec3{1, 2, 3},
        5.0f
    }.center
    == Vec3{1, 2, 3});

static_assert(
    Sphere{
        Vec3{1, 2, 3},
        5.0f
    }.radius
    == 5.0f);

static_assert(
    Sphere{
        Vec3{1, 2, 3},
        5.0f
    }
    ==
    Sphere{
        Vec3{1, 2, 3},
        5.0f
    });