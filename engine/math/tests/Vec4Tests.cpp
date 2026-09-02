#if __APPLE__
   #include <catch2/catch_test_macros.hpp>
   #include <utility>
#else
    #include <catch2/catch_all.hpp>
#endif

#include "../include/Vec4.hpp"
using namespace yarep::math;

TEST_CASE("Vec4 empty constructor") {
    constexpr auto vec = Vec4();
    REQUIRE(vec.x == 0);
    REQUIRE(vec.y == 0);
    REQUIRE(vec.z == 0);
    REQUIRE(vec.w == 0);
}

TEST_CASE("Vec4 constructor") {
    constexpr auto a = Vec4(1, 2, 4, 8);
    REQUIRE(a.x == 1);
    REQUIRE(a.y == 2);
    REQUIRE(a.z == 4);
    REQUIRE(a.w == 8);

    constexpr auto b = Vec4(-13, -1.467f, -23, -2);
    REQUIRE(b.x == -13);
    REQUIRE(b.y == -1.467f);
    REQUIRE(b.z == -23);
    REQUIRE(b.w == -2);
}

TEST_CASE("Vec4 addition") {
    const Vec4& a{1.0f, 2.0f, 4.0f, 8.0f};
    const Vec4& b{3.0f, 4.0f, 5.0f, 6.0f};

    Vec4 result = a + b;
    REQUIRE(result == Vec4{4, 6, 9, 14});

    const Vec4& c{-4, -3, -2, -1};
    result = c + b;
    REQUIRE(result == Vec4{-1, 1, 3, 5});
}

TEST_CASE("Vec4 addition assignment") {
    Vec4 a{1.0f, 2.0f, 3.0f, 4.0f};
    constexpr Vec4 add{3.0f, 4.0f, 5.0f, 6.0f};

    a += add;
    REQUIRE(a == Vec4{4, 6, 8, 10});
    REQUIRE(add == Vec4{3, 4, 5, 6});

    Vec4 b{-4, -3, -2, -1};
    b += add;
    REQUIRE(b == Vec4{-1, 1, 3, 5});

    Vec4 c{3, 5, 7, 9};
    c += c;
    REQUIRE(c == Vec4{6, 10, 14, 18});
}

TEST_CASE("Vec4 subtraction") {
    const Vec4& a{1.0f, 2.0f, 3.0f, 4.0f};
    const Vec4& b{3.0f, 4.0f, 5.0f, 6.0f};
    Vec4 result = a - b;
    REQUIRE(result == Vec4{-2, -2, -2, -2});

    const Vec4& c{-4, -3, -2, -1};
    const Vec4& d{-4, -4, -4, -3};
    result = c - d;
    REQUIRE(result == Vec4{0, 1, 2, 2});

    result = b - d;
    REQUIRE(result == Vec4{7, 8, 9, 9});
}

TEST_CASE("Vec4 subtraction assignment") {
    Vec4 a{1.0f, 2.0f, 3.0f, 4.0f};
    Vec4 b{1.0f, 2.0f, 3.0f, 4.0f};
    constexpr Vec4 sub_pos{3.0f, 4.0f, 5.0f, 6.0f};
    a -= sub_pos;
    REQUIRE(a == Vec4{-2, -2, -2, -2});
    REQUIRE(sub_pos == Vec4{3, 4, 5, 6});

    constexpr Vec4 sub_neg{-4, -3, -2, -1};
    b -= sub_neg;
    REQUIRE(b == Vec4{5, 5, 5, 5});

    Vec4 c{3, 5, 7, 9};
    c -= c;
    REQUIRE(c == Vec4{0, 0, 0, 0});
}

TEST_CASE("Vec4 multiplication") {
    const Vec4& vector{1.0f, 2.0f, 3.0f, 4.0f};
    const Vec4& zero{0.0f, 0.0f, 0.0f, 0.0f};
    constexpr float scalar = 3;
    constexpr float neg_scalar = -2;
    constexpr float nil_scalar = 0;

    Vec4 result = vector * scalar;
    REQUIRE(result == Vec4{3, 6, 9, 12});
    result = scalar * vector;
    REQUIRE(result == Vec4{3, 6, 9, 12});

    result = vector * neg_scalar;
    REQUIRE(result == Vec4{-2, -4, -6, -8});

    result = vector * nil_scalar;
    REQUIRE(result == Vec4{0, 0, 0, 0});

    result = zero * scalar;
    REQUIRE(result == Vec4{0, 0, 0, 0});

    result = zero * neg_scalar;
    REQUIRE(result == Vec4{0, 0, 0, 0});
}

TEST_CASE("Vec4 multiplication assignment") {
    Vec4 first{1.0f, 2.0f, 3.0f, 4.0f};
    Vec4 second{1.0f, 2.0f, 3.0f, 4.0f};
    Vec4 third{1.0f, 2.0f, 3.0f, 4.0f};
    Vec4 zero{0.0f, 0.0f, 0.0f, 0.0f};

    constexpr float scalar = 3;
    constexpr float neg_scalar = -2;
    constexpr float nil_scalar = 0;

    first *= scalar;
    REQUIRE(first == Vec4{3, 6, 9, 12});

    second *= neg_scalar;
    REQUIRE(second == Vec4{-2, -4, -6, -8});

    third *= nil_scalar;
    REQUIRE(third == Vec4{0, 0, 0, 0});

    zero *= scalar;
    REQUIRE(zero == Vec4{0, 0, 0, 0});
}

TEST_CASE("Vec4 division") {
    const Vec4& vector{10.0f, 4.0f, 2.0f, 1.0f};
    const Vec4& zero{0.0f, 0.0f, 0.0f, 0.0f};
    constexpr float scalar = 2;
    constexpr float neg_scalar = -2;
    constexpr float nil_scalar = 0;

    Vec4 result = vector / scalar;
    REQUIRE(result == Vec4{5, 2, 1, 0.5});

    result = vector / neg_scalar;
    REQUIRE(result == Vec4{-5, -2, -1, -0.5});

    REQUIRE_THROWS(vector / nil_scalar);

    result = zero / scalar;
    REQUIRE(result == Vec4{0, 0, 0, 0});
}

TEST_CASE("Vec4 division assignment") {
    Vec4 first{10.0f, 4.0f, 2.0f, 1.0f};
    Vec4 second{10.0f, 4.0f, 2.0f, 1.0f};
    Vec4 third{10.0f, 4.0f, 2.0f, 1.0f};
    Vec4 fourth{0, 0, 0, 0};

    constexpr float scalar = 2;
    constexpr float neg_scalar = -2;
    constexpr float nil_scalar = 0;

    first /= scalar;
    REQUIRE(first == Vec4{5, 2, 1, 0.5});

    second /= neg_scalar;
    REQUIRE(second == Vec4{-5, -2, -1, -0.5});

    REQUIRE_THROWS(third /= nil_scalar);
    REQUIRE(third == Vec4{10, 4, 2, 1});

    fourth /= scalar;
    REQUIRE(fourth == Vec4{0, 0, 0, 0});
}

TEST_CASE("Vec4 unary") {
    const Vec4& positive{1.0f, 2.0f, 3.0f, 4.0f};
    const Vec4& negative{-1.0f, -2.0f, -3.0f, -4.0f};
    const Vec4& zero{0.0f, 0.0f, 0.0f, 0.0f};
    const Vec4& mixed{-1.0f, 1.0f, -1.0f, 1.0f};

    auto result_positive = -positive;
    REQUIRE(result_positive == Vec4{-1, -2, -3, -4});

    auto result_negative = -negative;
    REQUIRE(result_negative == Vec4{1, 2, 3, 4});

    auto result_zero = -zero;
    REQUIRE(result_zero == Vec4{0, 0, 0, 0});

    auto result_mixed = -mixed;
    REQUIRE(result_mixed == Vec4{1, -1, 1, -1});
}

TEST_CASE("Vec4 equals") {
    const Vec4& a{1, 2, 3, 4};
    const Vec4& b{1, 2, 3, 4};
    const Vec4& c{1, 2, 3, 4.000001f};

    bool a_eq_b = a == b;
    REQUIRE(a_eq_b);

    bool a_eq_c = a == c;
    REQUIRE_FALSE(a_eq_c);
}

static_assert(Vec4{1.0f, 2.0f, 3.0f, 4.0f} + Vec4{3.0f, 4.0f, 5.0f, 6.0} == Vec4{4.0f, 6.0f, 8.0f, 10.0f});
static_assert(Vec4{2.0f, 3.0f, 4.0f, 5.0f} * 2.0f == Vec4{4.0f, 6.0f, 8.0f, 10.0f});
