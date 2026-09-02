#if __APPLE__
   #include <catch2/catch_test_macros.hpp>
   #include <utility>
#else
    #include <catch2/catch_all.hpp>
#endif

#include "../include/Vec3.hpp"
using namespace yarep::math;

TEST_CASE("Vec3 empty constructor") {
    constexpr auto vec = Vec3();
    REQUIRE(vec.x == 0);
    REQUIRE(vec.y == 0);
    REQUIRE(vec.z == 0);
}

TEST_CASE("Vec3 constructor") {
    constexpr auto a = Vec3(1, 2, 4);
    REQUIRE(a.x == 1);
    REQUIRE(a.y == 2);
    REQUIRE(a.z == 4);

    constexpr auto b = Vec3(-13, -1.467f, -23);
    REQUIRE(b.x == -13);
    REQUIRE(b.y == -1.467f);
    REQUIRE(b.z == -23);
}

TEST_CASE("Vec3 addition") {
    const Vec3& a{1.0f, 2.0f, 4.0f};
    const Vec3& b{3.0f, 4.0f, 5.0f};

    Vec3 result = a + b;
    REQUIRE(result == Vec3{4, 6, 9});

    const Vec3& c{-4, -3, -2};
    result = c + b;
    REQUIRE(result == Vec3{-1, 1, 3});
}

TEST_CASE("Vec3 addition assignment") {
    Vec3 a{1.0f, 2.0f, 3.0f};
    constexpr Vec3 add{3.0f, 4.0f, 5.0f};

    a += add;
    REQUIRE(a == Vec3{4, 6, 8});
    REQUIRE(add == Vec3{3, 4, 5});

    Vec3 b{-4, -3, -2};
    b += add;
    REQUIRE(b == Vec3{-1, 1, 3});

    Vec3 c{3, 5, 7};
    c += c;
    REQUIRE(c == Vec3{6, 10, 14});
}

TEST_CASE("Vec3 subtraction") {
    const Vec3& a{1.0f, 2.0f, 3.0f};
    const Vec3& b{3.0f, 4.0f, 5.0f};
    Vec3 result = a - b;
    REQUIRE(result == Vec3{-2, -2, -2});

    const Vec3& c{-4, -3, -2};
    const Vec3& d{-4, -4, -4};
    result = c - d;
    REQUIRE(result == Vec3{0, 1, 2});

    result = b - d;
    REQUIRE(result == Vec3{7, 8, 9});
}

TEST_CASE("Vec3 subtraction assignment") {
    Vec3 a{1.0f, 2.0f, 3.0f};
    Vec3 b{1.0f, 2.0f, 3.0f};
    constexpr Vec3 sub_pos{3.0f, 4.0f, 5.0f};
    a -= sub_pos;
    REQUIRE(a == Vec3{-2, -2, -2});
    REQUIRE(sub_pos == Vec3{3, 4, 5});

    constexpr Vec3 sub_neg{-4, -3, -2};
    b -= sub_neg;
    REQUIRE(b == Vec3{5, 5, 5});

    Vec3 c{3, 5, 7};
    c -= c;
    REQUIRE(c == Vec3{0, 0, 0});
}

TEST_CASE("Vec3 multiplication") {
    const Vec3& vector{1.0f, 2.0f, 3.0f};
    const Vec3& zero{0.0f, 0.0f, 0.0f};
    constexpr float scalar = 3;
    constexpr float neg_scalar = -2;
    constexpr float nil_scalar = 0;

    Vec3 result = vector * scalar;
    REQUIRE(result == Vec3{3, 6, 9});
    result = scalar * vector;
    REQUIRE(result == Vec3{3, 6, 9});

    result = vector * neg_scalar;
    REQUIRE(result == Vec3{-2, -4, -6});

    result = vector * nil_scalar;
    REQUIRE(result == Vec3{0, 0, 0});

    result = zero * scalar;
    REQUIRE(result == Vec3{0, 0, 0});

    result = zero * neg_scalar;
    REQUIRE(result == Vec3{0, 0, 0});
}

TEST_CASE("Vec3 multiplication assignment") {
    Vec3 first{1.0f, 2.0f, 3.0f};
    Vec3 second{1.0f, 2.0f, 3.0f};
    Vec3 third{1.0f, 2.0f, 3.0f};
    Vec3 zero{0.0f, 0.0f, 0.0f};

    constexpr float scalar = 3;
    constexpr float neg_scalar = -2;
    constexpr float nil_scalar = 0;

    first *= scalar;
    REQUIRE(first == Vec3{3, 6, 9});

    second *= neg_scalar;
    REQUIRE(second == Vec3{-2, -4, -6});

    third *= nil_scalar;
    REQUIRE(third == Vec3{0, 0, 0});

    zero *= scalar;
    REQUIRE(zero == Vec3{0, 0, 0});
}

TEST_CASE("Vec3 division") {
    const Vec3& vector{10.0f, 4.0f, 2.0f};
    const Vec3& zero{0.0f, 0.0f, 0.0f};
    constexpr float scalar = 2;
    constexpr float neg_scalar = -2;
    constexpr float nil_scalar = 0;

    Vec3 result = vector / scalar;
    REQUIRE(result == Vec3{5, 2, 1});

    result = vector / neg_scalar;
    REQUIRE(result == Vec3{-5, -2, -1});

    REQUIRE_THROWS(vector / nil_scalar);

    result = zero / scalar;
    REQUIRE(result == Vec3{0, 0, 0});
}

TEST_CASE("Vec3 division assignment") {
    Vec3 first{10.0f, 4.0f, 2.0f};
    Vec3 second{10.0f, 4.0f, 2.0f};
    Vec3 third{10.0f, 4.0f, 2.0f};
    Vec3 fourth{0, 0, 0};

    constexpr float scalar = 2;
    constexpr float neg_scalar = -2;
    constexpr float nil_scalar = 0;

    first /= scalar;
    REQUIRE(first == Vec3{5, 2, 1});

    second /= neg_scalar;
    REQUIRE(second == Vec3{-5, -2, -1});

    REQUIRE_THROWS(third /= nil_scalar);
    REQUIRE(third == Vec3{10, 4, 2});

    fourth /= scalar;
    REQUIRE(fourth == Vec3{0, 0, 0});
}

TEST_CASE("Vec3 unary") {
    const Vec3& positive{1.0f, 2.0f, 3.0f};
    const Vec3& negative{-1.0f, -2.0f, -3.0f};
    const Vec3& zero{0.0f, 0.0f, 0.0f};
    const Vec3& mixed{-1.0f, 1.0f, -1.0f};

    auto result_positive = -positive;
    REQUIRE(result_positive == Vec3{-1, -2, -3});

    auto result_negative = -negative;
    REQUIRE(result_negative == Vec3{1, 2, 3});

    auto result_zero = -zero;
    REQUIRE(result_zero == Vec3{0, 0, 0});

    auto result_mixed = -mixed;
    REQUIRE(result_mixed == Vec3{1, -1, 1});
}

TEST_CASE("Vec3 equals") {
    const Vec3& a{1, 2, 3};
    const Vec3& b{1, 2, 3};
    const Vec3& c{1.0000001f, 2, 3};

    bool a_eq_b = a == b;
    REQUIRE(a_eq_b);

    bool a_eq_c = a == c;
    REQUIRE_FALSE(a_eq_c);
}

static_assert(Vec3{1.0f, 2.0f, 3.0f} + Vec3{3.0f, 4.0f, 5.0f} == Vec3{4.0f, 6.0f, 8.0f});
static_assert(Vec3{2.0f, 3.0f, 4.0f} * 2.0f == Vec3{4.0f, 6.0f, 8.0f});
