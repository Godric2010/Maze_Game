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
if (BUILD_TESTING)
    include(testing)

    file(GLOB TEST_SOURCES
            CONFIGURE_DEPENDS
            "${CMAKE_CURRENT_SOURCE_DIR}/tests/*.cpp")
    add_catch2_tests(
            TARGET Utilities_tests
            PREFIX "utilities."
            LINK Utilities 
            SOURCES ${TEST_SOURCES}
    )

endif ()
