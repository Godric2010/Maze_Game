

add_library(Core STATIC
        EngineController.cpp
        EngineController.hpp)

target_include_directories(Core
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/../window/include
)
target_link_libraries(Core PUBLIC Window)