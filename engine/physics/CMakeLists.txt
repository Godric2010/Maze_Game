cmake_minimum_required(VERSION 3.31)

find_package(glm CONFIG REQUIRED)

add_library(Physics STATIC
        include/math/Types.hpp
        src/math/Overlap.cpp
        src/math/ClosestPoint.cpp
        include/math/ClosestPoint.hpp
        src/math/Sweep.cpp
        include/math/Sweep.hpp
        src/math/Resolve.cpp
        include/math/Resolve.hpp
        src/collision/SpatialHashBroadphase.cpp
        src/collision/SpatialHashBroadphase.hpp
        include/collision/IBroadphase.hpp
        tests/SweepTests.cpp
        include/collision/MoverSolver.hpp
        tests/MoverSolverTests.cpp
        tests/FakeCollisionQueryService.hpp
        tests/TypeUtiliesTests.cpp
        include/math/TypeUtils.hpp
        include/collision/CollisionUtils.hpp
)

if (COMMAND enable_coverage)
    message("Physics- Enable coverage")
    enable_coverage(Physics)
endif ()

target_include_directories(Physics
        PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

target_link_libraries(Physics PRIVATE Interface glm::glm)

if (BUILD_TESTING)
    include(testing)

    file(GLOB TEST_SOURCES
            CONFIGURE_DEPENDS
            "${CMAKE_CURRENT_SOURCE_DIR}/tests/*.cpp")
    add_catch2_tests(
            TARGET Physics_tests
            PREFIX "physics."
            LINK Physics
            SOURCES ${TEST_SOURCES}
    )

endif ()