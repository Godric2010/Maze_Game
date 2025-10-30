
add_library(Core STATIC
        EngineController.cpp
        EngineController.hpp
        components/Transform.hpp
        components/Mesh.hpp
        components/Camera.hpp
        systems/TransformSystem.cpp
        systems/TransformSystem.hpp
        systems/CameraSystem.cpp
        systems/CameraSystem.hpp
        ServiceLocator.inl
        ServiceLocator.hpp
        systems/RenderSystem.cpp
        systems/RenderSystem.hpp
        IEngine.hpp
        GameWorld.hpp
        systems/EngineEventSystem.cpp
        systems/EngineEventSystem.hpp
        components/MotionIntent.hpp
        systems/physics/PhysicsSystem.cpp
        systems/physics/PhysicsSystem.hpp
        components/Collider.hpp
        ../physics/include/collision/ColliderCache.hpp
        ../physics/include/collision/CollisionQueryService.hpp
        components/ui/Image.hpp
        components/ui/RectTransform.hpp
        InputManager.cpp
        InputManager.hpp
)

target_include_directories(Core PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/../environment/include
        ${CMAKE_CURRENT_SOURCE_DIR}/components
        ${CMAKE_CURRENT_SOURCE_DIR}/systems
)
target_link_libraries(Core PUBLIC Window Renderer ECS Physics)