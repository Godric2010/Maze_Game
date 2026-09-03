#if __APPLE__
   #include <catch2/catch_test_macros.hpp>
   #include <utility>
#else
    #include <catch2/catch_all.hpp>
#endif

#include "../include/Quaternion.hpp"

using namespace yarep::math;

TEST_CASE("Quaternion empty constructor") {
    constexpr Quaternion quaternion{};

    REQUIRE(quaternion.x == 0);
    REQUIRE(quaternion.y == 0);
    REQUIRE(quaternion.z == 0);
    REQUIRE(quaternion.w == 1);
}

TEST_CASE("Quaternion constructor") {
    constexpr Quaternion quaternion{
        1.0f,
        2.0f,
        3.0f,
        4.0f
    };

    REQUIRE(quaternion.x == 1);
    REQUIRE(quaternion.y == 2);
    REQUIRE(quaternion.z == 3);
    REQUIRE(quaternion.w == 4);

    constexpr Quaternion negative{
        -1.0f,
        -2.0f,
        -3.0f,
        -4.0f
    };

    REQUIRE(negative.x == -1);
    REQUIRE(negative.y == -2);
    REQUIRE(negative.z == -3);
    REQUIRE(negative.w == -4);
}

TEST_CASE("Quaternion identity") {
    constexpr Quaternion identity =
        Quaternion::identity();

    REQUIRE(identity.x == 0);
    REQUIRE(identity.y == 0);
    REQUIRE(identity.z == 0);
    REQUIRE(identity.w == 1);
}

TEST_CASE("Quaternion identity multiplication") {
    constexpr Quaternion quaternion{
        1.0f,
        2.0f,
        3.0f,
        4.0f
    };

    constexpr Quaternion identity =
        Quaternion::identity();

    constexpr Quaternion left =
        identity * quaternion;

    constexpr Quaternion right =
        quaternion * identity;

    REQUIRE(left == quaternion);
    REQUIRE(right == quaternion);
}

TEST_CASE("Quaternion multiplication") {
    constexpr Quaternion a{
        1.0f,
        2.0f,
        3.0f,
        4.0f
    };

    constexpr Quaternion b{
        5.0f,
        6.0f,
        7.0f,
        8.0f
    };

    constexpr Quaternion result = a * b;

    REQUIRE(result == Quaternion{
        24.0f,
        48.0f,
        48.0f,
        -6.0f
    });
}

TEST_CASE("Quaternion multiplication is not commutative") {
    constexpr Quaternion a{
        1.0f,
        2.0f,
        3.0f,
        4.0f
    };

    constexpr Quaternion b{
        5.0f,
        6.0f,
        7.0f,
        8.0f
    };

    constexpr Quaternion ab = a * b;
    constexpr Quaternion ba = b * a;

    REQUIRE_FALSE(ab == ba);
}

TEST_CASE("Quaternion multiplication assignment") {
    Quaternion quaternion{
        1.0f,
        2.0f,
        3.0f,
        4.0f
    };

    constexpr Quaternion rhs{
        5.0f,
        6.0f,
        7.0f,
        8.0f
    };

    quaternion *= rhs;

    REQUIRE(quaternion == Quaternion{
        24.0f,
        48.0f,
        48.0f,
        -6.0f
    });

    REQUIRE(rhs == Quaternion{
        5.0f,
        6.0f,
        7.0f,
        8.0f
    });
}

TEST_CASE("Quaternion self multiplication assignment") {
    Quaternion quaternion{
        1.0f,
        2.0f,
        3.0f,
        4.0f
    };

    quaternion *= quaternion;

    REQUIRE(quaternion == Quaternion{
        8.0f,
        16.0f,
        24.0f,
        2.0f
    });
}

TEST_CASE("Quaternion equals") {
    constexpr Quaternion a{
        1.0f,
        2.0f,
        3.0f,
        4.0f
    };

    constexpr Quaternion b{
        1.0f,
        2.0f,
        3.0f,
        4.0f
    };

    constexpr Quaternion c{
        1.0f,
        2.0f,
        3.0f,
        4.000001f
    };

    REQUIRE(a == b);
    REQUIRE_FALSE(a == c);
}

static_assert(
    Quaternion::identity()
    == Quaternion{0.0f, 0.0f, 0.0f, 1.0f}
);

static_assert(
    Quaternion::identity()
    * Quaternion{1.0f, 2.0f, 3.0f, 4.0f}
    == Quaternion{1.0f, 2.0f, 3.0f, 4.0f}
);