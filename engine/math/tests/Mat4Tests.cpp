#if __APPLE__
   #include <catch2/catch_test_macros.hpp>
   #include <utility>
#else
    #include <catch2/catch_all.hpp>
#endif

#include "../include/Mat4.hpp"

using namespace yarep::math;

TEST_CASE("Mat4 empty constructor") {
    constexpr Mat4 mat{};

    REQUIRE(mat(0, 0) == 0);
    REQUIRE(mat(0, 1) == 0);
    REQUIRE(mat(0, 2) == 0);
    REQUIRE(mat(0, 3) == 0);

    REQUIRE(mat(1, 0) == 0);
    REQUIRE(mat(1, 1) == 0);
    REQUIRE(mat(1, 2) == 0);
    REQUIRE(mat(1, 3) == 0);

    REQUIRE(mat(2, 0) == 0);
    REQUIRE(mat(2, 1) == 0);
    REQUIRE(mat(2, 2) == 0);
    REQUIRE(mat(2, 3) == 0);

    REQUIRE(mat(3, 0) == 0);
    REQUIRE(mat(3, 1) == 0);
    REQUIRE(mat(3, 2) == 0);
    REQUIRE(mat(3, 3) == 0);
}

TEST_CASE("Mat4 constructor") {
    constexpr Mat4 mat{
        Vec4{1, 5, 9, 13},
        Vec4{2, 6, 10, 14},
        Vec4{3, 7, 11, 15},
        Vec4{4, 8, 12, 16}
    };

    REQUIRE(mat(0, 0) == 1);
    REQUIRE(mat(0, 1) == 2);
    REQUIRE(mat(0, 2) == 3);
    REQUIRE(mat(0, 3) == 4);

    REQUIRE(mat(1, 0) == 5);
    REQUIRE(mat(1, 1) == 6);
    REQUIRE(mat(1, 2) == 7);
    REQUIRE(mat(1, 3) == 8);

    REQUIRE(mat(2, 0) == 9);
    REQUIRE(mat(2, 1) == 10);
    REQUIRE(mat(2, 2) == 11);
    REQUIRE(mat(2, 3) == 12);

    REQUIRE(mat(3, 0) == 13);
    REQUIRE(mat(3, 1) == 14);
    REQUIRE(mat(3, 2) == 15);
    REQUIRE(mat(3, 3) == 16);
}

TEST_CASE("Mat4 value access") {
    Mat4 mat{};

    mat(0, 0) = 1;
    mat(1, 3) = 5;
    mat(2, 1) = -3;
    mat(3, 2) = 8;

    REQUIRE(mat(0, 0) == 1);
    REQUIRE(mat(1, 3) == 5);
    REQUIRE(mat(2, 1) == -3);
    REQUIRE(mat(3, 2) == 8);
}

TEST_CASE("Mat4 identity") {
    constexpr Mat4 mat = Mat4::identity();

    REQUIRE(mat(0, 0) == 1);
    REQUIRE(mat(1, 1) == 1);
    REQUIRE(mat(2, 2) == 1);
    REQUIRE(mat(3, 3) == 1);

    REQUIRE(mat(0, 1) == 0);
    REQUIRE(mat(0, 2) == 0);
    REQUIRE(mat(0, 3) == 0);

    REQUIRE(mat(1, 0) == 0);
    REQUIRE(mat(1, 2) == 0);
    REQUIRE(mat(1, 3) == 0);

    REQUIRE(mat(2, 0) == 0);
    REQUIRE(mat(2, 1) == 0);
    REQUIRE(mat(2, 3) == 0);

    REQUIRE(mat(3, 0) == 0);
    REQUIRE(mat(3, 1) == 0);
    REQUIRE(mat(3, 2) == 0);
}

TEST_CASE("Mat4 vector multiplication") {
    constexpr Mat4 mat{
        Vec4{1, 5, 9, 13},
        Vec4{2, 6, 10, 14},
        Vec4{3, 7, 11, 15},
        Vec4{4, 8, 12, 16}
    };

    constexpr Vec4 vec{1, 2, 3, 4};

    constexpr Vec4 result = mat * vec;

    REQUIRE(result == Vec4{30, 70, 110, 150});
}

TEST_CASE("Mat4 identity vector multiplication") {
    constexpr Vec4 vec{2, -3, 5, 7};

    constexpr Vec4 result = Mat4::identity() * vec;

    REQUIRE(result == vec);
}

TEST_CASE("Mat4 matrix multiplication") {
    constexpr Mat4 a{
        Vec4{1, 5, 9, 13},
        Vec4{2, 6, 10, 14},
        Vec4{3, 7, 11, 15},
        Vec4{4, 8, 12, 16}
    };

    constexpr Mat4 b{
        Vec4{16, 12, 8, 4},
        Vec4{15, 11, 7, 3},
        Vec4{14, 10, 6, 2},
        Vec4{13, 9, 5, 1}
    };

    constexpr Mat4 result = a * b;

    constexpr Mat4 expected{
        Vec4{80, 240, 400, 560},
        Vec4{70, 214, 358, 502},
        Vec4{60, 188, 316, 444},
        Vec4{50, 162, 274, 386}
    };

    REQUIRE(result == expected);
}

TEST_CASE("Mat4 identity matrix multiplication") {
    constexpr Mat4 mat{
        Vec4{1, 5, 9, 13},
        Vec4{2, 6, 10, 14},
        Vec4{3, 7, 11, 15},
        Vec4{4, 8, 12, 16}
    };

    REQUIRE(mat * Mat4::identity() == mat);
    REQUIRE(Mat4::identity() * mat == mat);
}

TEST_CASE("Mat4 equals") {
    constexpr Mat4 a{
        Vec4{1, 5, 9, 13},
        Vec4{2, 6, 10, 14},
        Vec4{3, 7, 11, 15},
        Vec4{4, 8, 12, 16}
    };

    constexpr Mat4 b{
        Vec4{1, 5, 9, 13},
        Vec4{2, 6, 10, 14},
        Vec4{3, 7, 11, 15},
        Vec4{4, 8, 12, 16}
    };

    constexpr Mat4 c{
        Vec4{1, 5, 9, 13},
        Vec4{2, 6, 10, 14},
        Vec4{3, 7, 11, 15},
        Vec4{4, 8, 12, 17}
    };

    REQUIRE(a == b);
    REQUIRE_FALSE(a == c);
}

static_assert(
    Mat4::identity() * Vec4{1, 2, 3, 4}
    == Vec4{1, 2, 3, 4}
);

static_assert(
    Mat4::identity() * Mat4::identity()
    == Mat4::identity()
);