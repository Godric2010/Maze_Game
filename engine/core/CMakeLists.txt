
add_library(Core STATIC
        EngineController.cpp
        EngineController.hpp
        ServiceLocator.inl
        ServiceLocator.hpp
        ../interface/include/Commands/ui/ButtonClickedCommand.hpp
)

target_include_directories(Core PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)
target_link_libraries(Core PUBLIC Interface Components Systems SceneManagement Input Environment Text Renderer ECS Physics Debug)