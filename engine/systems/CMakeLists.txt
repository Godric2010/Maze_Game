cmake_minimum_required(VERSION 3.31)

find_package(glm)

add_library(Systems STATIC
        src/physics/PhysicsSystem.cpp
        include/PhysicsSystem.hpp
        src/camera/CameraSystem.cpp
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
        include/CacheManager.hpp
        src/camera/CameraCache.cpp
        src/camera/CameraCache.hpp
)

target_link_libraries(Systems PUBLIC Components Renderer Text Physics glm::glm Input )

target_include_directories(Systems
        PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>
        $<INSTALL_INTERFACE:include>
)
