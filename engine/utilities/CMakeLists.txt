cmake_minimum_required(VERSION 3.31)

find_package(tomlplusplus CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)

add_library(Utilities STATIC
        include/toml/TomlDocument.hpp
        src/toml/TomlDocument.cpp
)

target_link_libraries(Utilities PUBLIC tomlplusplus::tomlplusplus glm::glm spdlog::spdlog)

target_include_directories(Utilities PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)