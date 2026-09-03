#if __APPLE__
   #include <catch2/catch_test_macros.hpp>
   #include <utility>  
#else
    #include <catch2/catch_all.hpp>
#endif

#include "../include/Mat3.hpp"
using namespace yarep::math;

TEST_CASE("Mat3 empty constructor") {
    constexpr Mat3 mat{};

    REQUIRE(mat(0, 0) == 0);
    REQUIRE(mat(0, 1) == 0);
    REQUIRE(mat(0, 2) == 0);

    REQUIRE(mat(1, 0) == 0);
    REQUIRE(mat(1, 1) == 0);
    REQUIRE(mat(1, 2) == 0);

    REQUIRE(mat(2, 0) == 0);
    REQUIRE(mat(2, 1) == 0);
    REQUIRE(mat(2, 2) == 0);
}

TEST_CASE("Mat3 constructor") {
    constexpr Mat3 mat{
        Vec3{1, 4, 7},
        Vec3{2, 5, 8},
        Vec3{3, 6, 9}
    };

    REQUIRE(mat(0, 0) == 1);
    REQUIRE(mat(0, 1) == 2);
    REQUIRE(mat(0, 2) == 3);

    REQUIRE(mat(1, 0) == 4);
    REQUIRE(mat(1, 1) == 5);
    REQUIRE(mat(1, 2) == 6);

    REQUIRE(mat(2, 0) == 7);
    REQUIRE(mat(2, 1) == 8);
    REQUIRE(mat(2, 2) == 9);
}

TEST_CASE("Mat3 value access") {
    Mat3 mat{};

    mat(0, 0) = 1;
    mat(1, 2) = 5;
    mat(2, 1) = -3;

    REQUIRE(mat(0, 0) == 1);
    REQUIRE(mat(1, 2) == 5);
    REQUIRE(mat(2, 1) == -3);
}

TEST_CASE("Mat3 identity") {
    constexpr Mat3 mat = Mat3::identity();

    REQUIRE(mat(0, 0) == 1);
    REQUIRE(mat(1, 1) == 1);
    REQUIRE(mat(2, 2) == 1);

    REQUIRE(mat(0, 1) == 0);
    REQUIRE(mat(0, 2) == 0);
    REQUIRE(mat(1, 0) == 0);
    REQUIRE(mat(1, 2) == 0);
    REQUIRE(mat(2, 0) == 0);
    REQUIRE(mat(2, 1) == 0);
}

TEST_CASE("Mat3 vector multiplication") {
    constexpr Mat3 mat{
        Vec3{1, 4, 7},
        Vec3{2, 5, 8},
        Vec3{3, 6, 9}
    };

    constexpr Vec3 vec{1, 2, 3};

    constexpr Vec3 result = mat * vec;

    REQUIRE(result == Vec3{14, 32, 50});
}

TEST_CASE("Mat3 identity vector multiplication") {
    constexpr Vec3 vec{2, -3, 5};

    constexpr Vec3 result = Mat3::identity() * vec;

    REQUIRE(result == vec);
}

TEST_CASE("Mat3 matrix multiplication") {
    constexpr Mat3 a{
        Vec3{1, 4, 7},
        Vec3{2, 5, 8},
        Vec3{3, 6, 9}
    };

    constexpr Mat3 b{
        Vec3{9, 6, 3},
        Vec3{8, 5, 2},
        Vec3{7, 4, 1}
    };

    constexpr Mat3 result = a * b;

    constexpr Mat3 expected{
        Vec3{30, 84, 138},
        Vec3{24, 69, 114},
        Vec3{18, 54, 90}
    };

    REQUIRE(result == expected);
}

TEST_CASE("Mat3 identity matrix multiplication") {
    constexpr Mat3 mat{
        Vec3{1, 4, 7},
        Vec3{2, 5, 8},
        Vec3{3, 6, 9}
    };

    REQUIRE(mat * Mat3::identity() == mat);
    REQUIRE(Mat3::identity() * mat == mat);
}

TEST_CASE("Mat3 equals") {
    constexpr Mat3 a{
        Vec3{1, 4, 7},
        Vec3{2, 5, 8},
        Vec3{3, 6, 9}
    };

    constexpr Mat3 b{
        Vec3{1, 4, 7},
        Vec3{2, 5, 8},
        Vec3{3, 6, 9}
    };

    constexpr Mat3 c{
        Vec3{1, 4, 7},
        Vec3{2, 5, 8},
        Vec3{3, 6, 10}
    };

    REQUIRE(a == b);
    REQUIRE_FALSE(a == c);
}

static_assert(
    Mat3::identity() * Vec3{1, 2, 3}
    == Vec3{1, 2, 3}
);