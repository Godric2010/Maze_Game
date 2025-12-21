# Components

The components library is a collection of all engine-provided components. The library is publicly available from the engine, meaning that gameplay has 
access to it. Components are stored and owned by the ECS; therefore, they are managed by the [ECS Library](../ecs/Readme.md).
This library is a simple collection of all components provided by the engine and does not contain any kind of logic.

# Design rules
Every component in this library is a plain-old-data object. Meaning, it contains no logic at all with the exception being getter and setter functions.
Since it defines types that are used in gameplay and in the engine, it has no dependencies on any other library. The same counts for components themselves,
they have no dependencies on each other. Each component stands for itself. The components themselves have no lifetime management – the ECS does this.
Similar to the no dependency rule, is the No-Caching-Rule. Caching of data from components is managed in the [Systems Library](../systems/Readme.md).
Components only store data that is universally usable. System-dependent data is handled in the caches.

```C++
// This is a "good" component
struct MyComponent {
    private:
        int my_value = 0;
    
    public:
        MyComponent() {
            my_value = 0;
        }
    
        void SetMyValue(int new_value) {
            my_value = new_value;
        }
    
        int GetMyValue() {
            return my_value;
        }
};
```

## Ownership and mutability
Components are simple data storage objects. Gameplay and the engine can alter the data inside a component, but the interpretation of these values
is done by the systems only. A component without a system to use the data is therefore worthless.

## Non-Goals
Components are not supposed to do stuff like validation or serialization by themselves. This is part of the System Library. Also, these components are not supposed 
to support some sort of editor or debug features. To learn more about debug features, see the documentation [here](../debug/Readme.md).
Components don't need to take care of any sort of optimization, like layouting, storage or access patterns (getter and setters being the exception).
This is handled by the ECS Library.