cmake_minimum_required(VERSION 3.31)

find_package(glm)

add_library(Systems STATIC
        physics/PhysicsSystem.cpp
        physics/PhysicsSystem.hpp
        CameraSystem.cpp
        CameraSystem.hpp
        EngineEventSystem.hpp
        EngineEventSystem.cpp
        RectTransformSystem.cpp
        RectTransformSystem.hpp
        RenderSystem.cpp
        RenderSystem.hpp
        TransformSystem.cpp
        TransformSystem.hpp
        UiSystem.cpp
        UiSystem.hpp
)

target_link_libraries(Systems PUBLIC Renderer Text Physics glm::glm ECS Input Components)