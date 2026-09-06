#if __APPLE__
   #include <catch2/catch_test_macros.hpp>
   #include <utility>
#else
    #include <catch2/catch_all.hpp>
#endif

#include "../include/Transform.hpp"

using namespace yarep::math;


TEST_CASE("Transform empty constructor") {
    constexpr Transform transform{};

    REQUIRE(transform.position == Vec3{0, 0, 0});

    REQUIRE(
        transform.rotation
        == Quaternion::identity());

    REQUIRE(transform.scale == Vec3{1, 1, 1});
}


TEST_CASE("Transform constructor") {
    constexpr Transform transform{
        Vec3{1, 2, 3},
        Quaternion{4, 5, 6, 7},
        Vec3{8, 9, 10}
    };

    REQUIRE(transform.position == Vec3{1, 2, 3});

    REQUIRE(
        transform.rotation
        == Quaternion{4, 5, 6, 7});

    REQUIRE(transform.scale == Vec3{8, 9, 10});
}


TEST_CASE("Transform identity") {
    constexpr Transform transform =
        Transform::identity();

    REQUIRE(transform.position == Vec3{0, 0, 0});

    REQUIRE(
        transform.rotation
        == Quaternion::identity());

    REQUIRE(transform.scale == Vec3{1, 1, 1});
}


TEST_CASE("Transform equals") {
    constexpr Transform a{
        Vec3{1, 2, 3},
        Quaternion{0, 0, 0, 1},
        Vec3{2, 3, 4}
    };

    constexpr Transform b{
        Vec3{1, 2, 3},
        Quaternion{0, 0, 0, 1},
        Vec3{2, 3, 4}
    };

    constexpr Transform different_position{
        Vec3{2, 2, 3},
        Quaternion{0, 0, 0, 1},
        Vec3{2, 3, 4}
    };

    constexpr Transform different_rotation{
        Vec3{1, 2, 3},
        Quaternion{0, 1, 0, 0},
        Vec3{2, 3, 4}
    };

    constexpr Transform different_scale{
        Vec3{1, 2, 3},
        Quaternion{0, 0, 0, 1},
        Vec3{2, 3, 5}
    };

    REQUIRE(a == b);

    REQUIRE_FALSE(a == different_position);
    REQUIRE_FALSE(a == different_rotation);
    REQUIRE_FALSE(a == different_scale);
}


static_assert(
    Transform::identity().position
    == Vec3{0, 0, 0});

static_assert(
    Transform::identity().rotation
    == Quaternion::identity());

static_assert(
    Transform::identity().scale
    == Vec3{1, 1, 1});