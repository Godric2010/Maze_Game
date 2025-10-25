cmake_minimum_required(VERSION 3.31)

add_library(Gameplay STATIC
        GameplayManager.cpp
        GameplayManager.hpp
        systems/CameraControllerSystem.cpp
        systems/CameraControllerSystem.hpp
        mazegenerator/MazeAlgorithm.cpp
        mazegenerator/MazeAlgorithm.hpp
        mazegenerator/Cell.hpp
        mazegenerator/DebugGridDrawer.cpp
        mazegenerator/DebugGridDrawer.hpp
        MeshHandler.cpp
        MeshHandler.hpp
        systems/ItemSystem.cpp
        systems/ItemSystem.hpp
        components/Inventory.hpp
        components/KeyItem.hpp
        components/Exit.hpp)

target_link_libraries(Gameplay PUBLIC Engine)