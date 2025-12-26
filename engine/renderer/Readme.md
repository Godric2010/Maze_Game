# Renderer

The renderer library is an engine internal library, responsible for calculating the frame output on the
screen.
It abstracts the graphics API and provides a unified interface for the engine to render objects provided by the scene.
Important is here that the renderer library only interprets the data but never creates any objects or changes them.

## Scope

Renderer takes the objects that should be displayed from the renderer system, defined in [Systems](../systems/Readme.md)
and allocates all necessary resources, such as buffers for meshes and textures, as well as shaders.
It then renders the objects in their respected render pass and outputs the frame to its target.
Currently, only the screen as available as target.

Renderer does not communicate directly with the [Scene](../scenemanagement/Readme.md) or the [ECS](../ecs/Readme.md).
The Render System provides the objects already in the correct format, meaning that the renderer has no need to make any
transform calculations, and
gameplay is also not taken into account here.

## Architectural overview

The Renderer library contains two layers. The top layer is the abstraction layer used by the Render System to
query render requests. This is mostly implemented in the *Render Controller*.

The second layer is the render backend layer, implementing the OpenGL render API to communicate with the GPU.
Currently, the resources are allocated in the render backend, providing all necessary buffers to OpenGL to render
objects
according to their settings.

In the future, support for the Vulkan Graphics API is planned. This will also result in the resource management moving
to the
abstraction layer as far as possible.

## Render Pipeline and Render Passes

Currently, the render library is distincing between two object types. Both object types are rendered in separate render
passes.
First, all opaque objects are rendered in one render pass, proving an image buffer which the second render pass, for all
UI elements,
build upon.

Each render pass consumes the objects that are assigned to it and processes them on the GPU.
The separation of multiple render passes allows having a deterministic order in which various object types are drawn 
and ensures
that the UI elements are always rendered on top of all opaque objects.

Currently, only those two passes exist, but in the future more passes will be joining, like for transparent objects.
Also, the objects currently are only able to be rendered in unicolor. Support for textures and materials on opaque
objects is planned
in the future.

## Dataflow

The data required by the renderer library to display them comes from various sources.
Inside the [Components](../components/Readme.md), the data from meshes, transform and color (later textures, materials,
etc.) is stored.
In the [Systems](../systems/Readme.md) the data is cached and worked with, until the Render System finally pushes the
data into the renderer library for final processing. Since the render system is executed last in each tick (see [ECS](../ecs/Readme.md) for more details),
the display of the objects happens at the end of each tick as well.

A key point to note is that the renderer internally stores cached meshes in world space (and will also cache textures
and materials in the future), meaning the render system only needs to be informed about which objects to display. At
present, modifications to meshes and materials are not accommodated yet.

## API Abstraction
Abstracting OpenGL has many advantages. The main reason here was to be able to support other render APIs like Vulkan
or Metal later, without rewriting large chunks of the code. Furthermore, this keeps the rest of the engine clean since
references to OpenGL are not passed throughout the entire engine but are concentrated in the one spot where they are needed.

## Dependencies
- [Interface](../interface/Readme.md) - Accessing the entity ID datatype, will be removed soon
- Environment, for access to the window size, swap and context
- Open GL, through the GLAD package
- GLM, for math


## Non-Goals
The renderer library's only purpose is to render objects. It will never be responsible for editing materials or shader scripting. 
Furthermore, it is not supposed to have any runtime pipeline modifications or support scene graphing.


