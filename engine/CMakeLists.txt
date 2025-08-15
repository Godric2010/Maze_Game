
add_subdirectory(core)
add_subdirectory(environment)
add_subdirectory(renderer)

add_library(Engine INTERFACE)
target_link_libraries(Engine INTERFACE Core Window Renderer)