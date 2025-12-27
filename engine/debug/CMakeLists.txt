cmake_minimum_required(VERSION 3.31)

add_library(EngineDebug STATIC
        include/IDebugConsole.hpp
        src/DebugConsole.hpp
        src/DebugConsole.cpp
        include/DebugBuilder.hpp
)

target_link_libraries(EngineDebug PRIVATE Renderer Text Environment)
target_include_directories(EngineDebug PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>
        $<INSTALL_INTERFACE:include>
)
