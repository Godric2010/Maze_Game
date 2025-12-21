# The Engine

This is a modular, 3D engine written in C++, designed for learning and growing as an engine developer. It features a renderer, using OpenGL 4.1,
a custom entity-component-system and custom collision physics. It builds the foundation the [Maze Game](../README.md) is built upon.

It has no intention to be a competitor to big, AAA Engines out in the market. There is no Editor, custom scripting or any of that. 

The focus lies solely on the modularity and architecture, as well as the personal learning and growing options of me as an *Engine Developer*.
Therefore, this documentation targets other interested C++ and engine developers.

## Architecture overview and design principles

The engine is designed to keep a clear interface to the game using it. By implementing the *Engine* Library into a game project, the game gains access to
all high-level interfaces to communicate with the engine itself. This *Engine*-Library serves as an interface, so the game logic can use engine functions.
This allows the engine to hide internal functionality from gameplay, so no unintended calls can be made. Furthermore, this approach allows for a clear distinction
between engine functionality and engine usage.

*Interface* is the in-engine abstraction for many of the engines subsystems, allowing for clean access, without providing unnecessary engine details to the game.
*Components* is a collection of components, provided by the engine, to be used in gameplay.
*SceneManagement* allows the gameplay to register and switch between scenes

Internally, the engine is built out of many more libraries that are taking care of things but are not supposed to be accessible to the gameplay part.
All of those libraries, internally oder interface, follow the same setup.
A slim **include** directory, with interface classes, types and access points for the library user.
A **source** directory, containing all the libraries logic,
and a **tests** directory, where the unit tests for the library are built in.

Furthermore, the engine contains engine-only systems. Those work like regular systems, although they have more access to the engines' functionality.
This ensures that the engine can work the way it does, without breaking the path the entity component system lays out.
The ECS is in itself data-first orientated, with a very explicit update pipeline, ensuring that systems are dealt with in the right order. 
This allows working with in-engine and gameplay systems alike in one update loop.

## Engine Libraries

The engine contains multiple libraries that encapsulate various functionality of the engine, allowing for a modular setup and testing.

### [Core](core/Readme.md)
The core library is the *internal* library that is responsible for bootstrapping the other engines libraries and keeping the engine running until further notice.
It implements almost all libraries in engine, owns them and is responsible for their setup, updating and destruction. This makes core the library with dependencies to 
every other library in the engine.

### [ECS](ecs/Readme.md)
The ecs library is an *internal* library, responsible for managing the entities, their components and the systems in a scene. It relies on the in-engine systems
library, as well as the input and public *Interface* library. The library itself provides no access to the gameplay section.

### [Systems](systems/Readme.md)
The systems library is an *internal* library, containing all in-engine systems, like the render and physics system, that are needed to keep the engine running.
Furthermore, it contains the caches for the results of the systems that are used to share data in between systems. Naturally, it depends on the libraries needed 
to execute logic in the engine, such as the *Render* or the *Physics* library.

### [Components](components/Readme.md)
The components library is a *public* library, accessible in engine and gameplay. It provides the necessary component structs that store engine relevant data.
In there, components like the *Camera* or the *Transform* information are stored. Those components follow a strict "plain-old-data" policy. No component contains any kind of 
logic, with getter and setter functions for values being the exception.

### [Interface](interface/Readme.md)
The interface library is a *public* library, containing various datatypes and interfaces, that are provided to gameplay while concealing 
engine internal libraries and functionality.

### [Renderer](renderer/Readme.md)
The renderer library is an *internal* library that is responsible for the entire rendering process. Gameplay is not supposed to interact with rendering directly. 
Objects that should be rendered are controlled via the objects in the scene that have mesh components.
It uses the OpenGL framework to draw objects to the screen,
handles the meshes and shaders that are needed for rendering and features two render passes. One for Opaque objects and one for UI elements.

### [Input](input/Readme.md)
The input library is an *internal* library, responsible for managing the raw input and providing it to the active input map. It also manages the 
various input maps and input actions and preparing them for usage in the systems. For gameplay systems, the data is provided via the *Interface* library.

### [Physics](physics/Readme.md)
The physics library is an *internal* library, managing the calculation of collision physics in the engine. Physics shall never be controlled by gameplay directly.
To use physics results, override the corresponding functions in the gameplay system. It uses the *Interface* library to ensure
that the datatypes are the same as the gameplay section defined.

### [Scene Management](scenemanagement/Readme.md)
The scene management library is a *public* library, responsible for storing and switching scenes. Each scene switch creates a new world of the ECS and
therefore allows for a fresh start everytime a scene is changed. It does not define any scenes by itself. All scenes are provided by the gameplay. Scene management
is just responsible for keeping track of them. Since it has a heavy influence on it, the library relies on the *ECS*, *Input*, *Interface* and *Systems* library.

### [Debug](debug/Readme.md)
The debug library is an *internal* library, responsible for building debug output of the engine that gets rendered in the scene.
It relies heavily on the *UI* and *Render* library.

## How to: implement the engine into gameplay
To use the engine in a game, the *Engine* interface-library needs to be linked against the gameplay library. This way, access to all engine-defined components, all datatypes and interfaces in *Interface* and *scene management* is provided.
Link the gameplay library and the engine library, as well as the core library of the engine, against the executable and create the engine first, then the gameplay library.
This way, the gameplay stays clean, and the engine can be set up correctly by instantiating the Engine Controller.

The link order looks like this:

Gameplay → Interface → Engine

## How to: Add a new system
To add a new system, a new class needs to be created. On top of the class, the *ECS_SYSTEM* makro needs to be used, defining the systems name and 
position in the execution order. To learn more about the execution order of systems, refer to the [ECS Readme](ecs/Readme.md) 

If the system is a gameplay system, derive the class from *ISystem* and implement the initialize and run function. From here on the system is good to go.
If the system is an engine system, place the class in the systems library, derive it from *IEngineSystem* and implement the initialize and run functions.
Engine Systems are inherently more powerful, since they have access to engine internal libraries and functionalities, while gameplay systems only communicate of the 
engine interface, to ensure the distinction between "Engine Internal" and "Engine usage".

The systems don't need to be registered any further. On build-time, a scraper runs over all files, searching for the ECS_SYSTEM makro and then generating a 
class, where all systems from engine and gameplay get instantiated. This solution allows for a zero-registration implementation of the systems, making their usage and 
writing easier without needing to worry about missing references at any time.

## How to: Add a new engine library
To add a new library, create a new subdirectory in the engine directory. By convention, all library directories are lower case.
Inside this new library directory, create an include, src and optional a test directory.
For each library that it is highly encouraged to write Unit-Tests that should be stored in that test directory. High test coverage is
highly encouraged in this project to ensure functionality of each component without needing to rely on manual testing. 

Additionally, add a CMakeLists.txt file. State the name of the library in this CMake file. Note: Library names are UpperCase by convention.
To round of the basic setup, move to the CMakeLists.txt in the engine directory and add the new directory with add_subdirectory.

The structure should now look like this:
```bash
engine
├── libraryname
│   ├── CMakeLists.txt
│   ├── include
│   ├── src
│   └── tests
└── CMakeLists.txt
```

The new library is now ready to use. 

To make the files in include available for all library users, add the following statement underneath the add_library call
```CMake
target_include_directories(<Library>
    PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>
    $<INSTALL_INTERFACE:include>
)
```

To add the library to the coverage report, add the following statement to the CMakeLists.txt of the library:
```CMake
if (COMMAND enable_coverage)
    enable_coverage(<Library>)
endif ()

```

To add the library to CTest, add the following statement to the CMakeLists.txt. Remember that a test directory and at least one valid Catch2 test file are mandatory for this to work:
```CMake
if (BUILD_TESTING)
    include(testing)

    file(GLOB TEST_SOURCES
            CONFIGURE_DEPENDS
            "${CMAKE_CURRENT_SOURCE_DIR}/tests/*.cpp")
    add_catch2_tests(
            TARGET <Library>_tests
            PREFIX "<library>."     # Note: the prefix is by convention lower case
            LINK <Library>
            SOURCES ${TEST_SOURCES}
    )

endif ()
```
Tests that are added like this will be executed through GitHub Actions on every push of this project on the master branch.



