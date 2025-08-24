find_package(Python3 COMPONENTS Interpreter REQUIRED)

set(ECS_SCAN_DIRS
        ${CMAKE_SOURCE_DIR}/engine/core/systems
        ${CMAKE_SOURCE_DIR}/gameplay
)

message(STATUS "ECS Scan Dirs: ${ECS_SCAN_DIRS}")

file(GLOB_RECURSE ECS_SCAN_FILES
        CONFIGURE_DEPENDS
        ${ECS_SCAN_DIRS}/*.hpp
        ${ECS_SCAN_DIRS}/*.h
        ${ECS_SCAN_DIRS}/*.cpp
)

set(GENERATED_DIR ${CMAKE_SOURCE_DIR}/generated)
set(GENERATED_SRC ${GENERATED_DIR}/ECS_Systems.gen.cpp)

add_custom_target(ecs_codegen ALL
        COMMAND ${CMAKE_COMMAND} -E make_directory ${GENERATED_DIR}
        COMMAND ${Python3_EXECUTABLE} -u ${CMAKE_SOURCE_DIR}/tools/system_file_generator.py
        --roots ${ECS_SCAN_DIRS}
        --out ${GENERATED_SRC}
        BYPRODUCTS "${ECS_GENERATED_SRC}"
        DEPENDS ${ECS_SCAN_FILES} ${CMAKE_SOURCE_DIR}/tools/system_file_generator.py
        WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
        COMMENT "Generating ECS system registry..."
        VERBATIM)