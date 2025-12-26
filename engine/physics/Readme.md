# Physics

The physics library is an engine internal library. Its main purpose is to calculate the collision
of objects and return whether they are colliding or not. Therefore, this is a math-heavy library.
In the context of this engine, physics is referring only to collision physics. This is not a real physics simulation part.

## Scope
Physics is designed to determine whether two (or more) objects collide with each other.
It is able to detect such collisions and their resolve vector, uses a broad and narrow phase to find potential collision partners,
and can determine how far colliding objects overlap.

This can then be used by the Physics System, implemented in [Systems](../systems/Readme.md), to, for example, alter the position
of an object, based on that collision.

To make these calculations, the library uses colliders. Currently, box and sphere colliders are supported, more colliders will follow in the future.

Physics does not do anything beyond calculating the collision based on the provided data from the physics system. It does not manipulate these inputs,
it does not change anything in the engine. It is a simple request-answer library.

## Dependencies
Physics is very math-heavy and therefore uses the glm math library for all of its calculations. Furthermore, it depends on the
[Interface](../interface/Readme.md) library to gain access to the defined entities, which are used as keys for the collision caches.
Apart from these, the physics library is supposed to be independent of any other engine library. The dependency from interface will be removed
in future iterations of this library to ensure a clean, decoupled code base.

## Core Concept
Physics takes data from the physics system and uses its colliders to run first a broad- and then a narrow phase
to determine potential collisions. 
Currently, Box colliders (AABB, OBB) and Sphere colliders are supported.
This means that the colliders are analyzed against the target collider to filter out which colliders are potentially colliding.
The narrow phase in particular is then responsible for making a detailed analysis of the remaining colliders from the broad phase 
and determining whether they are really colliding or are just very narrow.

To nail it down, the potential colliders are tested for overlap, to get how much two colliders are overlapping.
The collider that is the best resolve candidate gets output in the collision result, along with the data on how deep
the collider penetration was. This makes the entire calculation very deterministic.

Physics itself never triggers any events! This is up to the physics system.

## Update flow
Since the physics system simply uses the physics library on call, the functions provided here are called whenever the physics
system demands it. To see a detailed overview on when which system is getting executed, refer to the [ECS Readme](../ecs/Readme.md)
Physics system is providing the collider structs of each object in world space. As described above, the collision then gets calculated
and the collision results are given back to the physics system. This includes the collision result including hit-normal, resolve vector, and the
Entity ID of the collision partner.

## Non-Goals
This library does not aim to calculate realistic physics or provide continued simulation of physics.
It is not supposed to know anything about gameplay or the engine itself besides from the colliders that it is supposed to use.
Also, it is never supposed to trigger any engine events on its own. This is all done by the physics system.

## Future
Rigidbody simulation, forces, masses, etc. are currently not taken care of in this library.
It is all very functionally implemented for now. In the future, more complex aspects are planned to be added to the library,
including forces, masses, and rigidbody simulation.

