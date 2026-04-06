cmake_minimum_required(VERSION 3.31)

add_library(Core STATIC
        EngineController.cpp
        EngineController.hpp
        ServiceLocator.inl
        ServiceLocator.hpp
        ../interface/include/Commands/UI/ButtonClickedCommand.hpp
        settings/Settings.hpp
        settings/SettingsHandler.cpp
        settings/SettingsHandler.hpp
)

target_include_directories(Core PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)
target_link_libraries(Core PUBLIC Interface Components Systems SceneManagement Input Environment Text Renderer ECS Physics AssetHandling EngineDebug Utilities )