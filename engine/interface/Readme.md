# Interface

The Interface library is a public library of the engine. It serves as the primary access point for gameplay to communicate with the engine 
by curating the access to engine-provided functions and datatypes. The interface library is designed with stability in mind, breaking
changed should only occur on major version bumps. Minor versions should only add to the library or mark obsolete functions as deprecated. 

## Scope
The library provides engine datatypes the gameplay is allowed to use, as well as functionality that allows reading data from the engine
or configuring parts of it. It is based on the *Facade* architectural pattern.
The types and functions are the public contract between the engine and gameplay.
Without these contracts, gameplay would not be able to create its own systems or scenes, for example.

## Lifetime and Ownership
Interface itself does not implement logic of any kind, this is done by the implementing libraries in the engine. This also counts for any kind
of data orchestration or lifetime-ownership. The engine libraries take care of this. 

## Public surface
The interface library provides access to various regions of the engine, separated into their respected namespaces.

- Commands: Commands sent by systems to configure functionality in the engine or gameplay
- ECS: Access to the ISystems header to derive gameplay systems from, as well as elementary datatypes 
- Input: Access to the various input types, like Input Maps, registration and consumption of input
- Renderer: Access to datatypes that are essential for configuring the renderer of the engine
- Application: Access to functions on the application level, like Quit.

## Design rules
Interface is designed to give curated access to the engine. No in-depth details of the engines' functionality should be visible here.
Furthermore, the library is designed to be small and lightweight. Except for dependencies to the glm library, no other dependencies should be added.
The goal is to have an extensible facade for the gameplay section that should be kept non-breaking if not a major version of the engine is released.
Datatypes should be kept minimalistic and small, following a similar plain-old-data pattern as it is seen in the [Components Library](../components/Readme.md).

In summary: Gameplay should be able to configure the engine via this Interface Library, while Engine in itself defines and executes the logic.

## How to...

### ... add and change a scene
To add a new scene, make sure that your scene class is deriving from IScene. Each scene can take some scene arguments in its constructor. 
The best practice here is to bundle up all arguments for a scene in one struct and pass it into the constructor as an argument. 
At a central class, use the *IApplication* interface to register the scene in the engine. Without that, the scene will not be detected.

To change to another scene, use the SceneManager access property in your current scene to tell the scene manager to load a new scene by its name.
Note that this will create a new scene, the old scene will be discarded after the change was successful. Also, the arguments for the new scene need to
be provided here. To learn more about scene management, consult the [Scene Management Library Readme](../scenemanagement/Readme.md)

###  ... add a new system
As already described in the [Systems Library](../systems/Readme.md), gameplay systems don't need to be registered directly.
To mark a class as a system class, let it derive from ISystem and add the ECS_Systems macro on top of the class.
Gameplay systems are allowed to be executed in Update and Late Update. To learn more about the macro and update phases, consult the Systems library.
To learn more about how systems use components and entities, consult the [ECS Library Readme](../ecs/Readme.md).

### ... add an input map
To add an input map, define it in a central location and then use the *IApplication* interface to register the input map in the engine.
To access the input from the input map, consult the Input access property inside a system to read the current input data. To learn more about input management,
consult the [Input Library Readme](../input/Readme.md)

## Dependencies
Interface is designed to have as few dependencies to external libraries as possible. Currently, the glm library is the only dependency Interface has.
Many Engine Libraries implement Interface to work on the provided datatypes or define functionality that is provided by the Interface Library.

```
                       |--> renderer
                       |--> input
Gameplay -> Interface -|--> ecs
                       |--> scenemanagement
                       |--> ...
```

## Non-Goals
Gameplay is not supposed to have direct access to engine internal functionalities. Therefore, direct access to engine functionality is not allowed here.
Furthermore, Interface is not designed to provide endless tempaltes, but small, easy-to-grasp datatypes for configuration.
Additionally, Interface is designed to provide access points and datatypes; no service location or caching is allowed here. This is all handled by the
respected engin internal library.

[[back](../Readme.md)]