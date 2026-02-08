cmake_minimum_required(VERSION 3.31)

find_package(glm)
find_package(spdlog CONFIG REQUIRED)

add_library(Systems STATIC
        src/physics/PhysicsSystem.cpp
        src/physics/PhysicsSystem.hpp
        src/camera/CameraSystem.cpp
        src/camera/CameraSystem.hpp
        src/events/EngineEventSystem.hpp
        src/events/EngineEventSystem.cpp
        src/transform/RectTransformSystem.cpp
        src/transform/RectTransformSystem.hpp
        src/rendering/RenderSystem.cpp
        src/rendering/RenderSystem.hpp
        src/transform/TransformSystem.cpp
        src/transform/TransformSystem.hpp
        src/ui/UiSystem.cpp
        src/ui/UiSystem.hpp
        src/transform/TransformCache.cpp
        src/transform/TransformCache.hpp
        src/CacheManager.hpp
        src/camera/CameraCache.cpp
        src/camera/CameraCache.hpp
        src/ui/UiCache.cpp
        src/ui/UiCache.hpp
        include/ICacheManager.hpp
        src/CacheManagerFactory.cpp
        include/CacheManagerFactory.hpp
)

target_link_libraries(Systems PUBLIC Components Renderer Text Physics spdlog::spdlog glm::glm Input)

target_include_directories(Systems
        PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>
        $<INSTALL_INTERFACE:include>
)
