cmake_minimum_required(VERSION 3.31)

find_package(spdlog REQUIRED)
find_package(Freetype REQUIRED)

add_library(Text STATIC
        TextDummy.cpp
        TextDummy.hpp
        include/FontHandle.hpp
        include/TextMeshHandle.hpp
        include/TextParameters.hpp)

target_link_libraries(Text PRIVATE Environment Freetype::Freetype spdlog::spdlog)

if (COMMAND enable_coverage)
    message("Text- Enable coverage")
    enable_coverage(Text)
endif ()

target_include_directories(Text PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>
        $<INSTALL_INTERFACE:include>
)
