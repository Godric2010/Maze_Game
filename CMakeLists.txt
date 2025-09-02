cmake_minimum_required(VERSION 3.31)

if (DEFINED ENV{VCPKG_ROOT})
    cmake_path(SET _VCPKG_ROOT NORMALIZE "$ENV{VCPKG_ROOT}")
    set(CMAKE_TOOLCHAIN_FILE "${_VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake" CACHE FILEPATH "vcpkg toolchain")
elseif (EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake")
    cmake_path(SET _LOCAL_VCPKG NORMALIZE "${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake")
    set(CMAKE_TOOLCHAIN_FILE "${_LOCAL_VCPKG}" CACHE FILEPATH "vcpkg toolchain")
endif ()

project(Maze_Game VERSION 0.4.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

#if (NOT CMAKE_CXX_COMPILER_ID MATCHES "Clang|AppleClang|MSVC")
#    message(FATAL_ERROR "This project requires Clang or MSVC as C++ compiler.")
#endif ()

include(CTest)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
if(BUILD_TESTING)
    find_package(Catch2 CONFIG REQUIRED)
endif ()

add_subdirectory(engine)
add_subdirectory(gameplay)

include(ecs_system_codegen)

add_executable(Maze_Game main.cpp generated/Generated.hpp "${GENERATED_SRC}")
add_dependencies(Maze_Game ecs_codegen)
target_include_directories(Maze_Game PRIVATE "${GENERATED_DIR}")


target_link_libraries(Maze_Game PRIVATE Engine Gameplay)

file(COPY ${CMAKE_SOURCE_DIR}/resources DESTINATION ${CMAKE_BINARY_DIR})

if (BUILD_TESTING)
    set(APP_REPORT_DIR "${CMAKE_CURRENT_BINARY_DIR}/test-reports" CACHE PATH "Dir for test reports")
    include(testing)

    set(_CTEST_CONFIG_ARG "")
    if (CMAKE_CONFIGURATION_TYPES)
        set(_CTEST_CONFIG_ARG "-C;$<CONFIG>")
    elseif (CMAKE_BUILD_TYPE)
        set(_CTEST_CONFIG_ARG "-C;${CMAKE_BUILD_TYPE}")
    endif ()
    set(CTEST_OUTPUT_ON_FAILURE ON)

    add_custom_target(check
            COMMAND ${CMAKE_CTEST_COMMAND}
            --output-on-failure
            --test-dir ${CMAKE_BINARY_DIR}
            ${_CTEST_CONFIG_ARG}
            USES_TERMINAL
            COMMENT "Running all tests via CTest")
endif ()