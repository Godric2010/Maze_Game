find_package(glad CONFIG REQUIRED)

add_library(Renderer STATIC include/Renderer.hpp include/RendererBuilder.hpp src/OpenGLRenderer.cpp)

target_include_directories(Renderer PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(Renderer PRIVATE Window glad::glad)
