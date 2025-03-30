# strictfp / stdfp / SF
Uses the same floating point implementation for all the version, by implementing a own floating point engine.

**_WARNING:_** This feature is recursive, so if you add it to a attribute, that isn't floating point it will make all floating point values in the Datatypes class strictfp


|Part|Support| Note|
|-|-|-|
| Compiler | X | Compiles a extra class into the project
| Interpreter | X | Uses a different class for this datatype
