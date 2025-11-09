cmake_minimum_required(VERSION 3.31)

if (DEFINED ENV{VCPKG_ROOT})
    cmake_path(SET _VCPKG_ROOT NORMALIZE "$ENV{VCPKG_ROOT}")
    set(CMAKE_TOOLCHAIN_FILE "${_VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake" CACHE FILEPATH "vcpkg toolchain")
elseif (EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake")
    cmake_path(SET _LOCAL_VCPKG NORMALIZE "${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake")
    set(CMAKE_TOOLCHAIN_FILE "${_LOCAL_VCPKG}" CACHE FILEPATH "vcpkg toolchain")
endif ()

project(Maze_Game VERSION 0.5.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)


include(CTest)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
if (BUILD_TESTING)
    find_package(Catch2 CONFIG REQUIRED)
endif ()

option(ENABLE_COVERAGE "Enable Coverage Flags" OFF)
function(enable_coverage TGT)
    if (NOT TARGET ${TGT})
        message(WARNING "enable_coverage: target '${TGT}' not found")
        return()
    endif ()
    if (NOT ENABLE_COVERAGE OR NOT CMAKE_CXX_COMPILER_ID MATCHES "Clang")
        return()
    endif ()

    get_target_property(_type ${TGT} TYPE)
    message("Enable coverage for ${TGT}")
    target_compile_options(${TGT} PRIVATE -O0 -g -fprofile-instr-generate -fcoverage-mapping)

    if (_type STREQUAL "EXECUTABLE" OR _type STREQUAL "SHARED_LIBRARY" OR _type STREQUAL "MODULE_LIBRARY")
        target_link_options(${TGT} PRIVATE -fprofile-instr-generate -fcoverage-mapping)
    endif ()
endfunction()

include(ecs_system_codegen)

add_subdirectory(engine)
add_subdirectory(gameplay)

add_executable(Maze_Game
        main.cpp
        "${GENERATED_CPP}"
)
add_dependencies(Maze_Game ecs_codegen)
target_include_directories(Maze_Game PRIVATE "${GENERATED_FILES_DIR}")
target_link_libraries(Maze_Game PRIVATE Engine Gameplay)

add_custom_command(TARGET Maze_Game POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/resources $<TARGET_FILE_DIR:${PROJECT_NAME}>/resources
        COMMENT "Copying resource folder...")

if (BUILD_TESTING)
    set(APP_REPORT_DIR "${CMAKE_CURRENT_BINARY_DIR}/test-reports" CACHE PATH "Dir for test reports")
    include(testing)

    set(_CTEST_CONFIG_ARG "")
    if (CMAKE_CONFIGURATION_TYPES)
        set(_CTEST_CONFIG_ARG "-C;$<CONFIG>")
    elseif (CMAKE_BUILD_TYPE)
        set(_CTEST_CONFIG_ARG "-C;${CMAKE_BUILD_TYPE}")
    endif ()
    set(CTEST_OUTPUT_ON_FAILURE ON)

    add_custom_target(check
            COMMAND ${CMAKE_CTEST_COMMAND}
            --output-on-failure
            --test-dir ${CMAKE_BINARY_DIR}
            ${_CTEST_CONFIG_ARG}
            USES_TERMINAL
            COMMENT "Running all tests via CTest")
endif ()


enable_coverage(Maze_Game)
# --- Coverage Target ---
find_program(LLVM_PROFDATA llvm-profdata)
find_program(LLVM_COV llvm-cov)

if (NOT LLVM_PROFDATA OR NOT LLVM_COV)
    message(WARNING "llvm-profdata/llvm-cov not found in PATH; coverage target may fail locally")
endif ()

set(COVERAGE_EXECUTABLES)

add_custom_target(coverage
        COMMAND ${CMAKE_COMMAND} -E env
        ENABLE_COVERAGE=${ENABLE_COVERAGE}
        ${CMAKE_SOURCE_DIR}/scripts/coverage.sh
        ${CMAKE_BINARY_DIR}
        ${COVERAGE_EXECUTABLES}
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
        USES_TERMINAL
        COMMENT "Running coverage via scripts/coverage.sh"
)