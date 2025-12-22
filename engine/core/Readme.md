# Core

Core is the bootstrapping library of the engine. It serves as a central owner of most of the libraries instances and provides
the main loop the engine resolves around. Core is engine-internal, providing no access to the outside. 
To access functionality in core from gameplay, use the *IApplication* interface, provided in the [Interface Library](../interface/Readme.md).

## Responsibilities
Core initializes the engine relevant libraries and holds their references, including all engine internal runtime-libraries.
Furthermore, Core provides access to those references with the help of the Service Locator. This allows systems to easily access other library instance references
and tigger functionality of them. Core populates it with the required library references. 

Additionally, core handles the update loop of the engine. Which library is updated at which point in time for each tick is defined
here.

## Design rules
Core is a dependency sink, meaning that core knows almost all engine libraries, and references them, but no other library should know about core.
It is responsible for orchestrating the libraries but implements not details about engine functionality itself. 
When it comes to gameplay-related data, core does not know about them. It only forwards access to the respected libraries, without validating or altering
any data in the process.

## Integration
Core is only integrated in the overarching project directly. This can create an instance of an engine controller and start up the engine this way.
Note that it is important that the engine is always started before calling any gameplay-related functionality. These are most likely dependent on an
existing and running engine instance. As a little caviar, core needs all systems that the engine shall use, as a constructor argument.
After an engine controller has been created, it is not possible to register any systems anymore. This is, by design, to ensure a deterministic and immutable usage of 
systems after start-up and allows for consistency with the generated code for system registration.

## Non-Goals
Core is not designed to host any detailed logic about engine functionality. It is the literal core of the engine that orchestrates logic.
Access functions should only forward data to the respected library, validation and modification of the data is done there.