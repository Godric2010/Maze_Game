cmake_minimum_required(VERSION 3.31)

add_library(Input STATIC
        include/IInputManager.hpp
        src/InputManager.cpp
        src/InputManager.hpp
        src/InputMapping.hpp
        include/InputManagerBuilder.hpp
)

target_link_libraries(Input PUBLIC Environment Interface)

if (COMMAND enable_coverage)
    message("Input- Enable coverage")
    enable_coverage(Input)
endif ()

target_include_directories(Input PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>
        $<INSTALL_INTERFACE:include>
)
if (BUILD_TESTING)
    include(testing)

    file(GLOB TEST_SOURCES
            CONFIGURE_DEPENDS
            "${CMAKE_CURRENT_SOURCE_DIR}/tests/*.cpp")
    add_catch2_tests(
            TARGET INPUT_tests
            PREFIX "input."
            LINK Input
            SOURCES ${TEST_SOURCES}
    )

endif ()
