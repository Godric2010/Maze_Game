cmake_minimum_required(VERSION 3.31)

add_library(ECS STATIC
        src/ComponentManager.cpp
        src/ComponentManager.hpp
        include/IComponent.hpp
)

target_include_directories(ECS
        PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

if (BUILD_TESTING)
    include(testing)
    add_catch2_tests(
            TARGET ECS_tests
            PREFIX "ecs."
            LINK ECS
            SOURCES tests/ComponentTests.cpp
    )

endif ()

