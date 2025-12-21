# Systems

The systems library is an engine-internal library, providing all necessary systems for the engine to run. Those systems use data from the 
[ECS Library](../ecs/Readme.md) to build logic upon them. 

## Scope
The systems library contains **engine-systems** only. Gameplay systems are not defined here but in the gameplay library itself.
Engine systems can be defined for each phase. To learn more about the phases, consult the [ECS Library Readme](../ecs/Readme.md).
Each engine system has a wide variety of access points. They have full access to the world-class without any abstraction layer in between, 
as well as access to the Service Provider, defined in [Core](../core/Readme.md) and the Cache Manager, provided by this library itself.

### Engine systems
Engine systems are special in the way that they have more access to engine internal functionalities than regular systems.
This allows the engine to run both engine dependant and gameplay dependant code in a single update loop, without the need to manage dependencies between
engine and gameplay. While regular systems, sometimes referred to as gameplay systems, are changing based on the type of game that is developed, engine
systems stay persistent for every project. They handle the engines logic by using the data from the components to perform operations on other engine
internal libraries. To underline this, each engine system derives from IEngineSystem, which in itself is a derivative of ISystem, the interface regular systems use.

## Update phase and execution order
The engine has a strict order in which systems are executed, defined by the execution type in the ECS_Systems macro.
Each tick executes all phases after another. 
1. Input – Pumping the input from the environment and prepare it
2. Physics – Calculate the collisions of objects
3. Ui - Manage all UI elements.
4. Update – General update step, also used by gameplay systems
5. EngineEvents – Deal with all internal engine events, such as Component or Entity creating/destruction events
6. LateUpdate – Updates that should be done after all updates are done, also used by gameplay systems
7. Commands – Manage systems commands and forward them to all system external listeners
8. Render – Render the scene

Each update phase has at least one system assigned to it; some have multiple systems. The order of execution inside a phase is defined
by the order the systems got detected by the code generation. This is done for simplicity reasons only so far; more detailed manipulation of the execution order
is planned in the future.

## System Context
Engine Systems have full access to the world as well as the Service Provider and the Cache Manager. They are bound once during initialization and kept stable reference for their entire lifetime.
The service provider allows the systems to call in references to other libraries, like, for example, the renderer library, to push data into those 
libraries for further processing. The Cache Manager allows engine systems to access cached component data with read and write access.
Furthermore, do engine systems have access to the command pipeline, being able to read and push to it, while regular gameplay systems are only allowed to push commands.


## Caches
Caches in the System Library allow systems to exchange data calculated from components. Since components only store data and perform no calculations, this falls to 
the system. But [Components](../components/Readme.md) should only hold values that are useful for the engine and gameplay side. Data that is relevant engine-only is stored in the caches.
An example here would be the transform matrix. Each Entity with a Transform component stores the position, rotation and scale. But for rendering the object at a certain position and rotation, a transform matrix is needed. 
This matrix is stored in the caches, so other systems can use it later down the update path.

The caches live as long as the world does. At a world reset, all caches get cleared as well. Each cache has one entry per entity. The entity is the identifier. If an entity is created or removed that affects a certain cache, the entry inside the cache gets created/removed as well.
If a component is updated, the updated values are overwritten for that entity inside the cache. Caches are only available for engine systems. 

## Commands and Events
Commands and Events are used to push information from a system to the outer layer around the update loop. 
Events, on the other hand, work the other way around, informing systems outside the update loop about changes.

Commands can be sent by any system, engine system and regular gameplay system. The processing of these events is done centralized in the *Commands* Phase of the update loop
by a single system called *Command System*.
Events are dealt with in the *Engine Events* Phase and executed by the *Engine Events System*. 
This solution provides a deterministic and centralized point on where events and commands are processed in the update loop, following the centralized dispatch pattern.

## Rules and Non-Goals
Engine systems contain only engine logic, no gameplay logic here.
Registration of systems is not handled here; this is done by the code generator for the systems, using the macro.
Systems are ordered by phase but not explicitly inside the order phase.
Systems do not own components or caches. They only use them.

## Dependencies
The library depends on several other libraries. Naturally, this is a one-directional dependency to avoid cycle dependencies.

- ECS
- Components
- Renderer
- Physics
- Input
- Text