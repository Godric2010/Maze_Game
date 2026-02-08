cmake_minimum_required(VERSION 3.31)

find_package(glm)
find_package(lodepng)

add_library(AssetHandling STATIC
        include/AssetHandler.hpp
        include/AssetTypes.hpp
        src/AssetManager.inl
        src/AssetTypesImpl.hpp
        src/Mesh/MeshImporter.cpp
        src/Mesh/MeshImporter.hpp
        src/Textures/TextureImporter.cpp
        src/Textures/TextureImporter.hpp
)

target_link_libraries(AssetHandling PUBLIC Environment glm::glm PRIVATE lodepng )

target_include_directories(AssetHandling
        PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

if (BUILD_TESTING)
    include(testing)

    file(GLOB TEST_SOURCES
            CONFIGURE_DEPENDS
            "${CMAKE_CURRENT_SOURCE_DIR}/tests/*.cpp")
    add_catch2_tests(
            TARGET AssetHandling_tests
            PREFIX "assets."
            LINK AssetHandling
            SOURCES ${TEST_SOURCES}
    )

endif ()
