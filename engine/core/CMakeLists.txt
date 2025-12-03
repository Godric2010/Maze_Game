
add_library(Core STATIC
        EngineController.cpp
        EngineController.hpp
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
        systems/physics/PhysicsSystem.cpp
        systems/physics/PhysicsSystem.hpp
        ../components/Collider.hpp
        ../physics/include/collision/ColliderCache.hpp
        ../physics/include/collision/CollisionQueryService.hpp
        systems/UiSystem.cpp
        systems/UiSystem.hpp
        commands/ui/ButtonClickedCommand.hpp
        systems/RectTransformSystem.cpp
        systems/RectTransformSystem.hpp
)

target_include_directories(Core PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/../environment/include
        ${CMAKE_CURRENT_SOURCE_DIR}/components
        ${CMAKE_CURRENT_SOURCE_DIR}/systems
)
target_link_libraries(Core PUBLIC Interface Components SceneManagement Input Environment Text Renderer ECS Physics Debug)