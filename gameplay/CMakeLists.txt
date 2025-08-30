cmake_minimum_required(VERSION 3.31)

add_library(Gameplay STATIC
        GameplayManager.cpp
        GameplayManager.hpp
        CameraControllerSystem.cpp
        CameraControllerSystem.hpp)

target_link_libraries(Gameplay PUBLIC Engine)