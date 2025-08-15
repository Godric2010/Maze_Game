find_package(SDL2 CONFIG REQUIRED)
message(STATUS "SDL2 found: ${SDL2_FOUND}")
message(STATUS "SDL2_INCLUDE_DIRS: ${SDL2_INCLUDE_DIRS}")
message(STATUS "SDL2_LIBRARIES: ${SDL2_LIBRARIES}")

add_library(Window STATIC
        include/Window.hpp
        src/SDLWindow.cpp
        include/EnvironmentBuilder.hpp
        include/Input.hpp
        src/SDLInput.cpp
        src/SDLInput.hpp
)

target_include_directories(Window PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(Window PUBLIC
        SDL2::SDL2
)