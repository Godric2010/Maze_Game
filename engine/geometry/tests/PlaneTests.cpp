#if __APPLE__
    #include <catch2/catch_test_macros.hpp>
    #include <utility>
#else
    #include <catch2/catch_all.hpp>
#endif

#include "../include/Plane.hpp"

using namespace yarep::geometry;
using namespace yarep::math;


// ============================================================================
// construction
// ============================================================================

TEST_CASE("Plane default constructor creates zero plane") {
    constexpr Plane plane{};

    REQUIRE(plane.normal == Vec3{0, 0, 0});
    REQUIRE(plane.distance == 0.0f);
}


TEST_CASE("Plane constructor stores normal and distance") {
    constexpr Plane plane{
        Vec3{0, 1, 0},
        -5.0f
    };

    REQUIRE(plane.normal == Vec3{0, 1, 0});
    REQUIRE(plane.distance == -5.0f);
}


TEST_CASE("Plane constructor supports negative normal components") {
    constexpr Plane plane{
        Vec3{-1, 0, 0},
        3.0f
    };

    REQUIRE(plane.normal == Vec3{-1, 0, 0});
    REQUIRE(plane.distance == 3.0f);
}


TEST_CASE("Plane constructor preserves non normalized normal") {
    constexpr Plane plane{
        Vec3{0, 5, 0},
        -10.0f
    };

    REQUIRE(plane.normal == Vec3{0, 5, 0});
    REQUIRE(plane.distance == -10.0f);
}


TEST_CASE("Plane supports zero distance") {
    constexpr Plane plane{
        Vec3{0, 1, 0},
        0.0f
    };

    REQUIRE(plane.normal == Vec3{0, 1, 0});
    REQUIRE(plane.distance == 0.0f);
}


// ============================================================================
// equality
// ============================================================================

TEST_CASE("Plane equality recognizes identical planes") {
    constexpr Plane lhs{
        Vec3{0, 1, 0},
        -5.0f
    };

    constexpr Plane rhs{
        Vec3{0, 1, 0},
        -5.0f
    };

    REQUIRE(lhs == rhs);
}


TEST_CASE("Plane equality rejects different normal") {
    constexpr Plane lhs{
        Vec3{0, 1, 0},
        -5.0f
    };

    constexpr Plane rhs{
        Vec3{1, 0, 0},
        -5.0f
    };

    REQUIRE_FALSE(lhs == rhs);
}


TEST_CASE("Plane equality rejects different distance") {
    constexpr Plane lhs{
        Vec3{0, 1, 0},
        -5.0f
    };

    constexpr Plane rhs{
        Vec3{0, 1, 0},
        -6.0f
    };

    REQUIRE_FALSE(lhs == rhs);
}


TEST_CASE("Plane inequality recognizes different planes") {
    constexpr Plane lhs{
        Vec3{0, 1, 0},
        -5.0f
    };

    constexpr Plane rhs{
        Vec3{0, 1, 0},
        -6.0f
    };

    REQUIRE(lhs != rhs);
}


// ============================================================================
// value semantics
// ============================================================================

TEST_CASE("Plane copy preserves values") {
    constexpr Plane original{
        Vec3{-1, 2, -3},
        7.0f
    };

    constexpr Plane copy = original;

    REQUIRE(copy == original);
}


// ============================================================================
// constexpr contract
// ============================================================================

static_assert(
        Plane{}.normal
        == Vec3{0, 0, 0}
        );

static_assert(
        Plane{}.distance
        == 0.0f
        );

static_assert(
        Plane{
            Vec3{0, 1, 0},
            -5.0f
        }.normal
        == Vec3{0, 1, 0}
        );

static_assert(
        Plane{
            Vec3{0, 1, 0},
            -5.0f
        }.distance
        == -5.0f
        );

static_assert(
        Plane{
            Vec3{0, 1, 0},
            -5.0f
        }
        ==
        Plane{
            Vec3{0, 1, 0},
            -5.0f
        }
        );
