cmake_minimum_required(VERSION 3.31)

if(DEFINED ENV{VCPKG_ROOT})
    set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake")
elseif (EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake")
    set(CMAKE_TOOLCHAIN_FILE "${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake")
endif ()

project(Maze_Game)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(NOT CMAKE_CXX_COMPILER_ID MATCHES "Clang|AppleClang|MSVC")
    message(FATAL_ERROR "This project requires Clang or MSVC as C++ compiler.")
endif()

add_subdirectory(engine)

add_executable(Maze_Game main.cpp
        engine/window/include/Window.hpp)
target_link_libraries(Maze_Game PRIVATE Engine)