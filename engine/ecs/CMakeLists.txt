cmake_minimum_required(VERSION 3.31)

add_library(ECS STATIC
        src/ComponentManager.inl
        src/ComponentManager.hpp
        include/World.hpp
        src/EntityManager.cpp
        src/EntityManager.hpp
        src/World.cpp
        src/ComponentPool.inl
        src/ComponentPool.hpp
        src/Entity.hpp
)

target_include_directories(ECS
        PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

if (BUILD_TESTING)
    include(testing)

    file(GLOB TEST_SOURCES
            CONFIGURE_DEPENDS
            "${CMAKE_CURRENT_SOURCE_DIR}/tests/*.cpp")
    add_catch2_tests(
            TARGET ECS_tests
            PREFIX "ecs."
            LINK ECS
            SOURCES ${TEST_SOURCES}
    )

endif ()

