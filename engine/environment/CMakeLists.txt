find_package(SDL2 CONFIG REQUIRED)
message(STATUS "SDL2 found: ${SDL2_FOUND}")
message(STATUS "SDL2_INCLUDE_DIRS: ${SDL2_INCLUDE_DIRS}")
message(STATUS "SDL2_LIBRARIES: ${SDL2_LIBRARIES}")

add_library(Environment STATIC
        include/Window.hpp
        src/SDLWindow.cpp
        include/EnvironmentBuilder.hpp
        include/EnvInput.hpp
        src/SDLInput.cpp
        src/SDLInput.hpp
        src/Files/FileReader.cpp
        src/Files/FileReader.hpp
        include/IFileReader.hpp
        src/EnvironmentBuilder.cpp
        src/Files/ExecutablePath.hpp
)

target_include_directories(Environment PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        $<INSTALL_INTERFACE:include>
)

target_link_libraries(Environment PUBLIC
        SDL2::SDL2
)