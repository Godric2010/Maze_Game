
add_subdirectory(core)
add_subdirectory(environment)
add_subdirectory(renderer)
add_subdirectory(ecs)
add_subdirectory(physics)

add_library(Engine INTERFACE)
target_link_libraries(Engine INTERFACE Core Window Renderer ECS Physics)