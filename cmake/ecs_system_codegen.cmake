cmake_minimum_required(VERSION 3.20)
include_guard(GLOBAL)

# Set variables
set(CODEGEN_SCRIPT "${CMAKE_SOURCE_DIR}/tools/system_file_generator.py")
set(CODEGEN_ROOT_DIRS
        ${CMAKE_SOURCE_DIR}/engine/systems
        ${CMAKE_SOURCE_DIR}/gameplay
)

set(GENERATED_DIR "${CMAKE_SOURCE_DIR}/generated")
set(GENERATED_BASE_NAME "ECS_Systems")
set(GENERATED_CPP_FILE "${GENERATED_DIR}/${GENERATED_BASE_NAME}.gen.cpp")


# Find python package
find_package(Python3 COMPONENTS Interpreter REQUIRED)

# Create target directory
file(MAKE_DIRECTORY "${GENERATED_DIR}")

# Inputs for rebuild trigger
message(STATUS "ECS Scan Dirs: ${CODEGEN_ROOT_DIRS}")
file(GLOB_RECURSE CODEGEN_INPUTS
        CONFIGURE_DEPENDS
        ${CODEGEN_ROOT_DIRS}/*.hpp
        ${CODEGEN_ROOT_DIRS}/*.h
        ${CODEGEN_ROOT_DIRS}/*.cpp
        ${CODEGEN_ROOT_DIRS}/*.inl
)
set(CODEGEN_ARGS --out "${GENERATED_CPP_FILE}" --roots)
list(APPEND CODEGEN_ARGS ${CODEGEN_ROOT_DIRS})

message("Code gen args: ${CODEGEN_ARGS}")
# Trigger code gen scrip
add_custom_command(
        OUTPUT "${GENERATED_CPP_FILE}"
        COMMAND ${Python3_EXECUTABLE} -u "${CODEGEN_SCRIPT}" ${CODEGEN_ARGS}
        DEPENDS "${CODEGEN_SCRIPT}" "${CODEGEN_INPUTS}"
        COMMENT "Generating ECS systems registry ${GENERATED_CPP_FILE}"
        VERBATIM
)

# Define target and output parameters of script
add_custom_target(ecs_codegen
        DEPENDS "${GENERATED_CPP_FILE}"
)

set(GENERATED_CPP "${GENERATED_CPP_FILE}")
set(GENERATED_FILES_DIR "${GENERATED_DIR}")

