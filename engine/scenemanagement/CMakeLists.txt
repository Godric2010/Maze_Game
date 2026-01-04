cmake_minimum_required(VERSION 3.31)

find_package(spdlog CONFIG REQUIRED)
add_library(SceneManagement STATIC
        include/IScene.hpp
        include/SceneWorld.hpp
        src/SceneContext.hpp
        src/SceneRegistry.cpp
        src/SceneContext.hpp
        src/SceneRegistry.hpp
        src/SceneRegistry.cpp
        src/SceneManager.hpp
        src/SceneManager.cpp
        tests/MockedClasses.hpp
)

target_link_libraries(SceneManagement PUBLIC ECS Interface Input Systems spdlog::spdlog)

if (COMMAND enable_coverage)
    message("SceneManagement- Enable coverage")
    enable_coverage(SceneManagement)
endif ()

target_include_directories(SceneManagement
        PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

if (BUILD_TESTING)
    include(testing)

    file(GLOB TEST_SOURCES
            CONFIGURE_DEPENDS
            "${CMAKE_CURRENT_SOURCE_DIR}/tests/*.cpp"
            "${CMAKE_CURRENT_SOURCE_DIR}/tests/*.hpp"
    )
    add_catch2_tests(
            TARGET SceneManagement_tests
            PREFIX "scenemanagement."
            LINK SceneManagement
            SOURCES ${TEST_SOURCES}
    )

endif ()
