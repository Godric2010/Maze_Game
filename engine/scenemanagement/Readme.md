# Scene Management

The scene management is an engine internal library. It is responsible for registration and management of scenes. 
Scenes are provided with a context, granting the user of a scene access to various parts of the engine.

## Scenes
All scenes need to derive from the IScene Interface. Each scene has its own instance of World provided by the scene context.
To learn more about the *World* and the ECS, check the [ECS Readme](../ecs/Readme.md).
The scene context allows access to various functions of the engine, like the Input and world, as well as the scene manager itself.

Furthermore, IScene provides a couple of functions to override, that are executed on Start and End of its lifecycle, as well
as receive-access to system-sent commands.

## Scope
This library aims to manage scenes provided through the [Interface](../interface/Readme.md), deal with their creation, registration and destruction,
as well as switching between scenes. The scenes manage themselves, based on their context provided data. They offer limited access to the World,
via the *SceneWorld* facade, but are strictly separated from any kind of gameplay logic or asset loading. Therefore, this library is only a controller 
to manage the various scenes but implements no detail on what those scenes might do. To communicate with scene management from gameplay, the Interface library provides all needed
access points. This library shall remain engine internal.

## Lifecycle
Each scene lives through the same steps in its lifecycle.
- Registration: The scene is made known to the scene manager
- Initialize: The scene gets its context
- OnStart: Overridable function to execute logic at the start of the scene
- Update: Internal update that runs the systems update calls
- OnExit: Overridable function, called when the scene is marked for destruction
- UnloadScene: Clears all references, world, caches, etc.

## Switching a scene
To switch to a new scene, the scene manager needs to be informed that a new scene is requested. 
The old scene will then be systematically destroyed, erasing all objects from the world, clearing out all system caches and freeing resources that are no longer needed.
Then the new scene will be created with its scene arguments. 
This includes building a new World, new system caches and allocating resources needed to display this scene.

At the time, there is no way implemented to load scenes additive or to stream seamlessly from one scene to another. So far, scene management guarantees only one
active scene at a time.

## Dependencies
- [Input](../input/Readme.md)
- [ECS](../ecs/Readme.md)
- [Systems](../systems/Readme.md)
- [Interface](../interface/Readme.md)

All dependencies are directly linked to the scene capabilities of operation that are needed to keep it running in the engine.


## How to...

### ...register a new scene
To register a scene, create a class in gameplay and let it derive from IScene. If the scene requires specific data or arguments (for example some data from the previous scene),
create a struct that contains all these data and use it as a constructor argument for the scene. Implement overrides for all functions that might be needed.

Use the *IApplication* interface to register a new scene and provide a name to it. This name will be the identifier by which the scene can be called.

### ...switch a scene
To load a new scene, call the instance of ISceneManager and its load scene function. This is provided through the Interface library. 
Provide the name of the scene that should be loaded, as well as the data required in the scene arguments.

By the end of the current update cycle, the scene switch will be executed.