cmake_minimum_required(VERSION 3.31)

find_package(glm)

add_library(Systems STATIC
        src/physics/PhysicsSystem.cpp
        include/PhysicsSystem.hpp
        src/CameraSystem.cpp
        include/CameraSystem.hpp
        include/EngineEventSystem.hpp
        src/EngineEventSystem.cpp
        src/transform/RectTransformSystem.cpp
        include/RectTransformSystem.hpp
        src/RenderSystem.cpp
        include/RenderSystem.hpp
        src/transform/TransformSystem.cpp
        include/TransformSystem.hpp
        src/UiSystem.cpp
        include/UiSystem.hpp
        src/transform/TransformCache.cpp
        src/transform/TransformCache.hpp
)

target_link_libraries(Systems PUBLIC Renderer Text Physics glm::glm ECS Input Components)