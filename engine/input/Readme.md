# Input

Input is an engine internal library. It is responsible for managing the various input parameters pulled from the Environment,
regarding the application and input devices and put them into a form that is usable for the rest of the engine. 

## Scope
The input library is responsible for the management of inputs, mostly provided by input devices. Currently, mouse and keyboard
are supported. Additional input devices might follow in the future. Furthermore, it is responsible for providing 
application events from the window manager to further usage in the [Core Library](../core/Readme.md).

Both kinds of input do not get processed here but directed to the listeners that are interested. Therefore, the 
input from the user gets stored into the respected input map and input action and is made available in the [Systems](../systems/Readme.md)
and the [Scenes](../scenemanagement/Readme.md) via the [Interface](../interface/Readme.md) library. 

Input itself has no idea what is done with the input, but knows which input maps exist and which one is currently active.
It only provides the input buffer to the gameplay, which then can read it. Input buffers cannot be altered outside of this library,
they are read-only.

## Lifecycle
Input is pulled at the beginning of every tick, and the input buffer gets overwritten with each tick to ensure that
only the relevant data for this tick gets provided.

Input maps can be registered at runtime via the IApplication interface, providing new layouts on how the input gets used
in gameplay. 

Switching of input maps can be also done at runtime via the IInput interface. This is also the place to set the mouse visibility.

## Input Maps and Actions

Input maps are structs providing a set of actions that get populated with data to be used by systems or scenes.
Currently, only one input map can be active at a time. Additional input maps are not supported at the moment.
Each input action contains a name, by which it can be identified human reader friendly, as well as the key or mouse button
is bound to this action and the press state of the button. 

If the input pulled from the environment matches one of the actions, the action gets populated with data and can get polled by the system
or scene. To make this possible, the results of each pull from the environment are collected in an input buffer, than made 
available through the Interface library for further usage. Additionally, each input buffer gets the current mouse position and its delta
independently of the active input map.  

Multiple input maps can use the same actions and keybindings, but since only one input map can be active, only one of the currently 
active input map actions will be submitted.

## Dependencies
The input library is used widespreadly in the engine for various purposes. Therefore, it keeps its own dependencies small.
It depends only on the Environment library and the Interface library, as well as the glm math library.

## Non-Goals
The library is no direct event dispatcher. The whole library is input-pull-based. Furthermore, this library should never know
anything gameplay-related. How the maps, actions and buffers are used is defined and used in gameplay only.
Last but not least, this library is not responsible for the device abstraction. This is done in the environment library.