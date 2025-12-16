cmake_minimum_required(VERSION 3.31)

find_package(glm)

add_library(Interface INTERFACE
        include/IApplication.hpp
        include/Input/IInput.hpp
        include/Input/InputTypes.hpp
        include/Input/InputBuffer.hpp
        include/scene/SceneArgs.hpp
        include/scene/ISceneManager.hpp
        include/Ecs/ISystem.hpp
        include/Ecs/Types.hpp
        include/Renderer/Types.hpp
)

target_link_libraries(Interface INTERFACE glm::glm)
target_include_directories(Interface INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>
        $<INSTALL_INTERFACE:include>
)
