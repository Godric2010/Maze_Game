find_package(GLEW CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)
add_library(Renderer STATIC
        src/RenderController.cpp
        src/renderframework/Renderer.hpp
        src/renderframework/OpenGL/OpenGLRenderer.cpp
        include/Datatypes.hpp
        src/renderframework/OpenGL/Meshes/OpenGLMesh.hpp
        src/renderframework/OpenGL/Meshes/OpenGlMeshLibrary.cpp
        src/renderframework/OpenGL/Meshes/OpenGlMeshLibrary.hpp
        src/renderframework/OpenGL/OpenGLShaderManager.cpp
        src/renderframework/OpenGL/OpenGLShaderManager.hpp
        src/PrimitiveMeshes.hpp
        src/renderframework/OpenGL/Textures/OpenGLTextureLibrary.cpp
        src/renderframework/OpenGL/Textures/OpenGLTextureLibrary.hpp
        src/renderframework/OpenGL/Textures/OpenGLTexture.hpp
        include/IRenderController.hpp
        src/RenderControllerFactory.cpp
        include/RenderControllerFactory.hpp
        src/renderframework/materials/MaterialLibrary.cpp
        src/renderframework/materials/MaterialLibrary.hpp
        src/renderframework/materials/Material.hpp
        src/renderframework/OpenGL/OpenGlBinder.cpp
        src/renderframework/OpenGL/OpenGlBinder.hpp
        src/renderframework/IGpuMeshLibrary.hpp
        src/renderframework/IGpuTextureLibrary.hpp)

target_link_libraries(Renderer PUBLIC Interface PRIVATE Environment AssetHandling GLEW::GLEW glm::glm spdlog::spdlog)

target_include_directories(Renderer
        PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>
        $<INSTALL_INTERFACE:include>
)
