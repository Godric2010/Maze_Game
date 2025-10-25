cmake_minimum_required(VERSION 3.31)

add_library(ECS STATIC
        src/ComponentManager.inl
        src/ComponentManager.hpp
        include/World.hpp
        src/EntityManager.cpp
        src/EntityManager.hpp
        src/World.inl
        src/ComponentPool.inl
        src/ComponentPool.hpp
        src/Entity.hpp
        include/ISystem.hpp
        src/SystemManager.cpp
        include/SystemManager.hpp
        include/IServiceToEcsProvider.hpp
        src/buffer/EcsEvent.hpp
        include/ComponentEventBus.hpp
        include/PhysicsEventBus.hpp
        src/buffer/EventBuffer.inl
        src/buffer/EventBuffer.hpp
        src/buffer/PhysicsEvent.hpp
)

if(COMMAND enable_coverage)
    message("ECS- Enable coverage")
    enable_coverage(ECS)
endif ()

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

