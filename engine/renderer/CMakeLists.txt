find_package(glad CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)

add_library(Renderer STATIC
        RenderController.cpp
        shadermanagement/ShaderManager.cpp
        renderframework/Renderer.hpp
        renderframework/OpenGL/OpenGLRenderer.cpp
        shadermanagement/Shader.hpp
        meshmanagement/MeshManager.cpp
        meshmanagement/MeshManager.hpp
        meshmanagement/Mesh.hpp)

target_include_directories(Renderer PUBLIC RenderController.hpp)
target_link_libraries(Renderer PRIVATE Window glad::glad glm::glm)
