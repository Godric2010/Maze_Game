cmake_minimum_required(VERSION 3.31)

add_library(Gameplay STATIC
        TestSystem.hpp
        TestSystem.cpp
        GameplayManager.cpp
        GameplayManager.hpp)

target_link_libraries(Gameplay PUBLIC Engine)