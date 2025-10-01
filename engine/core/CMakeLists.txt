
add_library(Core STATIC
        EngineController.cpp
        EngineController.hpp
        components/Transform.hpp
        components/Mesh.hpp
        components/Camera.hpp
        components/InputReceiver.hpp
        systems/TransformSystem.cpp
        systems/TransformSystem.hpp
        systems/CameraSystem.cpp
        systems/CameraSystem.hpp
        systems/InputReceiver.cpp
        systems/InputReceiver.hpp
        ServiceLocator.inl
        ServiceLocator.hpp
        systems/RenderSystem.cpp
        systems/RenderSystem.hpp
        IEngine.hpp
        GameWorld.hpp
        systems/CommandSystem.cpp
        systems/CommandSystem.hpp
        components/MotionIntent.hpp
        systems/PhysicsSystem.cpp
        systems/PhysicsSystem.hpp
        components/Collider.hpp
        ColliderCache.hpp
)

target_include_directories(Core PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/../environment/include
        ${CMAKE_CURRENT_SOURCE_DIR}/components
        ${CMAKE_CURRENT_SOURCE_DIR}/systems
)
target_link_libraries(Core PUBLIC Window Renderer ECS Physics)