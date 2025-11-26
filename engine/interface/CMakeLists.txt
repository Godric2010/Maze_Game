cmake_minimum_required(VERSION 3.31)

add_library(Interface INTERFACE
        include/IApplication.hpp
)

target_include_directories(Interface INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>
        $<INSTALL_INTERFACE:include>
)
