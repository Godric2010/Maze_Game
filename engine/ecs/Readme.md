# Entity Component System

The ECS library is an engine internal library, responsible for managing the objects in the scene (entities), their components
and the systems that are executed on those components with an explicit update order. The entire library is data-orientated first.
This library provides only the logical parts, no explicit components or systems.

## Scope
The ECS library manages the creation, destruction and update process of all objects in the "world". The world is the central structure,
the library revolves around. Worlds can be used to, for example, create a new scene and populate it with objects. Furthermore, the library
handles the components that are attached to each entity (object) in the world. Additionally, it provides the system management, 
which has access to the entities and components to feed that into the systems to work with them. Last but not least, the ecs library provides various internal
event chains for adding/ removing components and entities or to provide physics updates from one system to another. Both event pipelines are handled internally in 
the ecs and are not accessible from the outside.

The system manager owns all systems and grants access to the world and the context of the systems, but it provides no system-logic.
Systems are simply updated in a strict order to avoid confusion and dangling behavior. Which phase a system is executed in, is defined by the 
system itself.
Since components are strict plain-old-data types, they have no logic inherently. 

The library works as a registry for entities, components and systems and provides access but does not execute any logic beyond that.
No input validation, scene switching or asset management here – this is a non-goal for this library.


## Dependencies
- [Interface](../interface/Readme.md)
- [Input](../input/Readme.md)
- [Systems](../systems/Readme.md)

This library is designed for engine internal usage only. To provide functionality to gameplay, the interface library provides
the necessary datatypes and interfaces.
The access to interface is necessary to populate the interfaces and datatypes that are visible to gameplay.
The access to systems is necessary to prepare and update the systems, so they can execute their logic internally.
The access to input is necessary to provide the input context to the system as needed.

## Concept
### World
The world is the central access point to everything entity and component related. It provides ways to create and destroy entities and 
components, as well as accessing them. Furthermore, it provides certain events for creating and destroying an entity or push physics events like 
OnCollisionEnter to the systems. World is kept public to the engine, but internal for gameplay. To access the world from the gameplay part, use the class
*SystemWorld*.

#### SystemWorld
*SystemWorld* is a wrapper around World, providing read and write access to the components and entities, but hiding all pipelines and events to ensure controlled mutability. 
Access to the event loops is restricted here. It is called like that, because it is supposed to be only used in the context of systems and their need to access the worlds entities and components.

### Entity
An entity is a number value built from an *uint64_t* datatype. It represents an object in the world and can contain multiple components.
Entities are the central identifier when it comes to work with various components in the scene. Entities themselves hold no data at all.

### Component
A component is a plain old data object, usually a struct with no logic, except for getter and setter functions, maybe. A component represents
a specific type of property that an object shall have. Examples would be the Transform data, which provides a position, rotation and scale within the world to the object.
To learn more about Components, check the documentation on them [here](../components/Readme.md)

### System
A system is a piece of logical code that uses the data from components and entities to execute logic on them. Each system must derive from ISystem and is called once per 
frame and has access to the world, the current delta time, as well as the input and physics events. Systems don't need to be registered explicitly. They are owned and managed by the engine
via the ECS system manager.
If a class is marked by the ECS_System Makro, the code generator will pick it up automatically on build time and make sure it gets instantiated.
Make sure that the system needs no constructor arguments! The code generator cannot work with those.

```C++
   ECS_SYSTEM(MySystem, Update, [])  
    class MySystem final : public Engine::Ecs::ISystem {
    public:
        MySystem() = default;

        ~MySystem() override = default;

        void Initialize() override;

        void Run(float delta_time) override;
    };
```

The first argument of the makro is the name of your system. Make sure to write it exactly as your class is called.
The second argument defines the place in the update cycle. Those places are available:
- Input
- Physics
- Ui
- Update
- EngineEvents
- LateUpdate
- Commands
- Render

Note that for gameplay systems, only Update and LateUpdate are available so far. The rest is used by Engine Systems.

#### Engine Systems
Engine Systems are like regular gameplay systems, but with more power over the engine itself. They communicate directly with world, instead of using the wrapper, 
have direct access to other libraries and execute logic on them. To learn more about Engine Systems, read the documentation [here](../systems/Readme.md)

