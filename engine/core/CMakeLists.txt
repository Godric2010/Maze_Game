
add_library(Core STATIC
        EngineController.cpp
        EngineController.hpp
        Transform.cpp
        Transform.hpp
        systems/EngineTestSystem.cpp
        systems/EngineTestSystem.hpp
)

target_include_directories(Core
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/../environment/include
)
target_link_libraries(Core PUBLIC Window Renderer ECS)