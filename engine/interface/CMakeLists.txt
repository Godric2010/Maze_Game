cmake_minimum_required(VERSION 3.31)

add_library(Interface INTERFACE
        include/IApplication.hpp
        include/Input/IInput.hpp
        include/Input/InputTypes.hpp
        include/Input/InputBuffer.hpp
        include/scene/SceneArgs.hpp
        include/scene/ISceneManager.hpp
)

target_include_directories(Interface INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>
        $<INSTALL_INTERFACE:include>
)
