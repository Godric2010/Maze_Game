cmake_minimum_required(VERSION 3.31)

find_package(spdlog REQUIRED)
find_package(Freetype CONFIG REQUIRED)
message(STATUS "Freetype found: ${FREETYPE_FOUND}")
message(STATUS "Freetype_INCLUDE_DIRS: ${FREETYPE_INCLUDE_DIRS}")
message(STATUS "Freetype_LIBRARIES: ${FREETYPE_LIBRARIES}")
get_target_property(_inc Freetype::Freetype INTERFACE_INCLUDE_DIRECTORIES)
get_target_property(_dbg Freetype::Freetype IMPORTED_LOCATION_DEBUG)
get_target_property(_rel Freetype::Freetype IMPORTED_LOCATION_RELEASE)
message(STATUS "FT target includes: ${_inc}")
message(STATUS "FT debug lib: ${_dbg}")
message(STATUS "FT release lib: ${_rel}")

add_library(Text STATIC
        include/FontHandle.hpp
        include/TextMeshHandle.hpp
        include/TextParameters.hpp
        src/TextController.cpp
        include/TextController.hpp
        src/fontrasterization/FontManager.cpp
        src/fontrasterization/FontManager.hpp
        src/fontrasterization/FontLibrary.hpp
        src/fontrasterization/FontFace.cpp
        src/fontrasterization/FontFace.hpp)

target_link_libraries(Text PRIVATE Environment Freetype::Freetype spdlog::spdlog)

if (COMMAND enable_coverage)
    message("Text- Enable coverage")
    enable_coverage(Text)
endif ()

target_include_directories(Text PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>
        $<INSTALL_INTERFACE:include>
)
