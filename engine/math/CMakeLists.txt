cmake_minimum_required(VERSION 3.31)
find_package(glm)

add_library(Math STATIC
        include/Vec2.hpp
)

target_link_libraries(Math PRIVATE glm::glm)

if (COMMAND enable_coverage)
    message("Math- Enable Coverage")
    enable_coverage(Math)
endif ()

target_include_directories(Math PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

if (BUILD_TESTING)
    include(testing)

    file(GLOB TEST_SOURCES
            CONFIGURE_DEPENDS
            "${CMAKE_CURRENT_SOURCE_DIR}/tests/*.cpp"
    )
    add_catch2_tests(
            TARGET MATH_tests
            PREFIX "math."
            LINK Math
            SOURCES ${TEST_SOURCES}
    )
endif ()