find_package(glad CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)
add_library(Renderer STATIC
        src/RenderController.cpp
        src/shadermanagement/ShaderManager.cpp
        src/renderframework/Renderer.hpp
        src/renderframework/OpenGL/OpenGLRenderer.cpp
        src/shadermanagement/Shader.hpp
        include/Datatypes.hpp
        src/renderframework/OpenGL/OpenGLMesh.hpp
        src/renderframework/OpenGL/OpenGLMeshManager.cpp
        src/renderframework/OpenGL/OpenGLMeshManager.hpp
        src/renderframework/OpenGL/OpenGLShaderManager.cpp
        src/renderframework/OpenGL/OpenGLShaderManager.hpp
        src/PrimitiveMeshes.hpp
        src/renderframework/OpenGL/OpenGLTextureManager.cpp
        src/renderframework/OpenGL/OpenGLTextureManager.hpp
        src/renderframework/OpenGL/OpenGLTexture.hpp
        include/IRenderController.hpp
        src/RenderControllerFactory.cpp
        include/RenderControllerFactory.hpp)

target_link_libraries(Renderer PRIVATE Environment glad::glad glm::glm spdlog::spdlog)

target_include_directories(Renderer
        PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>
        $<INSTALL_INTERFACE:include>
)
