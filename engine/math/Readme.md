# Math 

Math is a library that can be accessed internally of the engine but also in gameplay.
It is responsible for providing all necessary mathematical datatypes and functions needed
to make the engine and the game run.

## Scope
The math library is responsible for providing general purpose datatypes and functions.
It depends on the external glm library but is otherwise completely independent of other libraries.

The math library is not aware of what the math is actually used for. It is usage agnostic, outlining just the very
foundations of mathematical functions, that might be needed in various situations.

## Dependencies
Since the library is used widely across the engine, it has zero dependencies to other libraries, except for the external glm package.

## Non-Goals
The library is not claiming to be a complete math collection of anything resolving math. New functionality can be added over time if needed.
Also, there is no geometric calculations in here. Those are handled by the [Geometry library](../geometry/Readme.md)