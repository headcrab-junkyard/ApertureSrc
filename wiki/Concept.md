# Engine architecture design

### General

Magenta is a server-oriented game engine (it's "headless" by default and has no user-specific modules inside its core)  
With this approach, it's possible to use the core for dedicated server purposes without any additions or changes to the core. Any additional functionality could be provided via the plugin extensions (currently even subsystems support are implemented using the plugins). 

### Features

**(These features are TODO list that should be implemented)**
* Modern C++ and object-oriented design;
* Multithreading support;
* Extensible architecture;
* TODO

### Background

Magenta engine was initially based on partially-rewritten GoldSrc engine sources called OGS, which is a custom-made GoldSrc engine clone that's based on the open-sourced Quake engine sources (the same sources which were used for GoldSrc). Magenta is kinda branch of the OGS engine fork which was aimed at porting the original engine sources to Modern C++ and making it object-oriented.