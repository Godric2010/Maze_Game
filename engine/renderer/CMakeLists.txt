find_package(glad CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)
add_library(Renderer STATIC
        RenderController.cpp
        shadermanagement/ShaderManager.cpp
        renderframework/Renderer.hpp
        renderframework/OpenGL/OpenGLRenderer.cpp
        shadermanagement/Shader.hpp
        Datatypes.hpp
        renderframework/OpenGL/OpenGLMesh.hpp
        renderframework/OpenGL/OpenGLMeshManager.cpp
        renderframework/OpenGL/OpenGLMeshManager.hpp
        renderframework/OpenGL/OpenGLShaderManager.cpp
        renderframework/OpenGL/OpenGLShaderManager.hpp
        PrimitiveMeshes.hpp
        renderframework/OpenGL/OpenGLTextureManager.cpp
        renderframework/OpenGL/OpenGLTextureManager.hpp
        renderframework/OpenGL/OpenGLTexture.hpp)

target_include_directories(Renderer PUBLIC RenderController.hpp)
target_link_libraries(Renderer PRIVATE Environment glad::glad glm::glm spdlog::spdlog)
