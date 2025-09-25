cmake_minimum_required(VERSION 3.31)

find_package(glm CONFIG REQUIRED)

add_library(Physics STATIC
        include/Types.hpp
        src/Overlap.cpp
        src/ClosestPoint.cpp
        include/ClosestPoint.hpp
        src/Sweep.cpp
        include/Sweep.hpp
        src/Resolve.cpp
        include/Resolve.hpp
)

target_include_directories(Physics
        PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

target_link_libraries(Physics PRIVATE glm::glm)

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