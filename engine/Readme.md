# The Engine

This is a modular, 3D engine written in C++, designed for learning and growing as an engine developer. It features a renderer, using OpenGL 4.1,
a custom entity-component-system and custom collision physics.

It has no intention to be a competitor to big, AAA Engines out in the market. There is no Editor, custom scripting or any of that. 

The focus lies solely on the modularity and architecture, as well as the personal learning and growing options of me as an *Engine Developer*.

## Design Principals

The engine is designed to keep a clear interface to the game using it. By implementing the *Engine* Library into a game project, the game gains access to
all high-level interfaces to communicate with the engine itself. This *Engine*-Library serves as an interface, so the game logic can use engine functions.

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

The engine contains multiple libraries, that encapsulate various functionality of the engine, allowing for a modular setup and testing.

### Core
The core library is the *internal* library that is responsible for bootstrapping the other engines libraries and keep the engine running until further notice.
It implements almost all libraries in engine, owns them and is responsible for their setup, updating and destruction.

### ECS
The ecs library is an *internal* library, responsible for managing the entities, their components and the systems in a scene. It relies on the in-engine systems
library, as well as the input and public *Interface* library. The library itself provides no access to the gameplay section.

### Systems
The systems library is an *internal* library, containing all in-engine systems, like the render and physics system, that are needed to keep the engine running.
Furthermore, it contains the caches for the results of the systems that are used to share data in between systems. Naturally, it depends on the libraries needed 
to execute logic in the engine, such as the *Render* or the *Physics* library.

### Components
The components library is a *public* library, accessible in engine and gameplay. It provides the necessary component structs that store engine relevant data.
In there, components like the *Camera* or the *Transform* information are stored. Those components follow a strict "plain-old-data" policy. No component contains any kind of 
logic, with getter and setter functions for values being the exception.

### Interface
The interface library is a *public* library, containing various datatypes and interfaces, that are provided to gameplay while concealing 
engine internal libraries and functionality.

### Renderer
The renderer library is an *internal* library that is responsible for the entire rendering process. It utilizes the OpenGL framework to draw objects to the screen,
handles the meshes and shaders that are needed for rendering and features two render passes. One for Opaque objects and one for UI elements.

### Input
The input library is an *internal* library, responsible for managing the raw input and providing it to the active input map. It also manages the 
various input maps and input actions and preparing them for usage in the systems. For gameplay systems, the data is provided via the *Interface* library.

### Physics
The physics library is an *internal* library, managing the calculation of collision physics in the engine. It uses the *Interface* library to ensure
that the datatypes are the same as the gameplay section defined.

### Scene Management
The scene management library is a *public* library, responsible for storing and switching scenes. Each scene switch creates a new world of the ECS and
therefore allows for a fresh start everytime a scene is changed. It does not define any scenes by itself. All scenes are provided by the gameplay. Scene management
is just responsible for keeping track of them. Since it has a heavy influence on it, the library relies on the *ECS*, *Input*, *Interface* and *Systems* library.

### Debug
The debug library is an *internal* library, responsible for building debug output of the engine that gets rendered in the scene.
It relies heavily on the *UI* and *Render* library.

## How to: implement the engine into gameplay
To use the engine in a game, the *Engine* interface-library needs to be linked against the gameplay library. This way, access to all engine-defined components, all datatypes and interfaces in *Interface* and *scene management* is provided.
Link the gameplay library and the engine library, as well as the core library of the engine, against the executable and create the engine first, then the gameplay library.
This way, the gameplay stays clean, and the engine can be set up correctly by instantiating the Engine Controller.

## How to: Add a new system
To add a new system, a new class needs to be created. On top of the class the *ECS_SYSTEM* makro needs to be used, defining the systems name and 
position in the execution order. 

If the system is a gameplay system, derive the class from *ISystem* and implement the initialize and run function. From here on the system is good to go.
If the system is an engine system, place the class in the systems library, derive it from *IEngineSystem* and implement the initialize and run functions.

The systems don't need to be registered any further. On build-time, a scraper runs over all files, searching for the ECS_SYSTEM makro and then generating a 
class, where all systems from engine and gameplay get instantiated. This code-gen allows for maximum flexibility when it comes to working with systems.

## How to: Add a new engine library
