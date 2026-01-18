cmake_minimum_required(VERSION 3.31)

add_library(AssetHandling STATIC
        include/AssetHandler.hpp
        include/AssetTypes.hpp
        src/AssetManager.inl
        src/AssetTypesImpl.hpp
        src/Mesh/MeshImporter.cpp
        src/Mesh/MeshImporter.hpp
)

target_link_libraries(AssetHandling PUBLIC Environment)

target_include_directories(AssetHandling
        PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>
        $<INSTALL_INTERFACE:include>
)
