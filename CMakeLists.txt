cmake_minimum_required(VERSION 3.31)
project(Maze_Game)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(NOT CMAKE_CXX_COMPILER_ID MATCHES "Clang|AppleClang|MSVC")
    message(FATAL_ERROR "This project requires Clang or MSVC as C++ compiler.")
endif()

if(NOT CMAKE_TOOLCHAIN_FILE)
    if(${CMAKE_SYSTEM_NAME} STREQUAL "Darwin")
        set(CMAKE_TOOLCHAIN_FILE "${CMAKE_SOURCE_DIR}/cmake/toolchain_macOS.cmake")
    elseif (${CMAKE_SYSTEM_NAME} STREQUAL "Windows")
        set(CMAKE_TOOLCHAIN_FILE "${CMAKE_SOURCE_DIR}/cmake/toolchain_windows.cmake")
    endif ()
endif ()

add_executable(Maze_Game main.cpp)
