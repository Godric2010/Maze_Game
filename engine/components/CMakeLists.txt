cmake_minimum_required(VERSION 3.31)

add_library(Components INTERFACE
        Camera.hpp
        Collider.hpp
        Mesh.hpp
        MotionIntent.hpp
        Transform.hpp
        ui/Button.hpp
        ui/Image.hpp
        ui/RectTransform.hpp
        ui/Text.hpp
)

target_include_directories(Components
        INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}>
        $<INSTALL_INTERFACE:include>
)
