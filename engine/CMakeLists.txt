
add_subdirectory(core)
add_subdirectory(window)

add_library(Engine INTERFACE)
target_link_libraries(Engine INTERFACE Core Window)