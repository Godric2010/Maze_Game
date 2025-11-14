cmake_minimum_required(VERSION 3.31)

find_package(spdlog REQUIRED)
find_package(Freetype CONFIG REQUIRED)

add_library(Text STATIC
        src/TextController.cpp
        include/TextController.hpp
        src/FontManager.cpp
        src/FontManager.hpp
        src/FontLibrary.hpp
        src/FontFace.hpp
        include/Types.hpp
        src/Utf8Decoder.cpp
        src/Utf8Decoder.hpp
        src/LayoutEngine.cpp
        src/LayoutEngine.hpp
        src/TextMeshBuilder.cpp
        src/TextMeshBuilder.hpp)

target_link_libraries(Text PRIVATE Environment Freetype::Freetype spdlog::spdlog)

if (COMMAND enable_coverage)
    message("Text- Enable coverage")
    enable_coverage(Text)
endif ()

target_include_directories(Text PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>
        $<INSTALL_INTERFACE:include>
)
if (BUILD_TESTING)
    include(testing)

    file(GLOB TEST_SOURCES
            CONFIGURE_DEPENDS
            "${CMAKE_CURRENT_SOURCE_DIR}/tests/*.cpp")
    add_catch2_tests(
            TARGET TEXT_tests
            PREFIX "text."
            LINK Text
            SOURCES ${TEST_SOURCES}
    )

endif ()
