find_package(GLEW CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)
add_library(Renderer STATIC
        include/Datatypes.hpp
        src/PrimitiveMeshes.hpp
        include/RenderControllerFactory.hpp
        include/IRenderController.hpp
        src/RenderControllerFactory.cpp
        src/RenderController.cpp
        src/resources/IGpuMeshLibrary.hpp
        src/resources/IGpuTextureLibrary.hpp
        src/resources/IShaderLibrary.hpp 
        src/resources/IGpuMaterialLibrary.hpp
        src/renderframework/Renderer.hpp
        src/renderframework/OpenGL/OpenGlBinder.cpp
        src/renderframework/OpenGL/OpenGlBinder.hpp
        src/renderframework/OpenGL/OpenGLRenderer.cpp
        src/renderframework/OpenGL/Meshes/OpenGLMesh.hpp
        src/renderframework/OpenGL/Meshes/OpenGlMeshLibrary.cpp
        src/renderframework/OpenGL/Meshes/OpenGlMeshLibrary.hpp
        src/renderframework/OpenGL/Shaders/OpenGlShaderLibrary.cpp
        src/renderframework/OpenGL/Shaders/OpenGlShaderLibrary.hpp
        src/renderframework/OpenGL/Textures/OpenGLTextureLibrary.cpp
        src/renderframework/OpenGL/Textures/OpenGLTextureLibrary.hpp
        src/renderframework/OpenGL/Textures/OpenGLTexture.hpp
        src/renderframework/OpenGL/materials/OpenGlMaterialLibrary.cpp
        src/renderframework/OpenGL/materials/OpenGlMaterialLibrary.hpp
        src/renderframework/OpenGL/materials/OpenGlMaterial.hpp

)

target_link_libraries(Renderer PUBLIC Interface PRIVATE Environment AssetHandling GLEW::GLEW glm::glm spdlog::spdlog)

target_include_directories(Renderer
        PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>
        $<INSTALL_INTERFACE:include>
)
