This engine should support both GoldSrc and its own native way of interaction between the engine and clientdll/gamedll/gameui/filesystem modules:
* New client and game dll interaction is based on virtual C++ interfaces rather than C structs of function pointers;
* Interfaces are split by their responsibilities;
* Native GameUI is using a slightly modified version of GoldSrc interface;
* Native FileSystem interface contains different way of interacting with files and archives (and still WIP);
* Engine will use protocol 49 (which is WIP);
* Engine resource limits could be completely removed from Native modules, but for GoldSrc compatibility they should remain the same (there is possible solution to add a new cvar that will lock the resource limits to GS standards but I still unsure about that);
* Some legacy engine interfaces should be implemented the way they should access the other module and get an export from it (example: Career UI interaction function inside the engine<->clientdll interface);

The framework, in this case, is something like "intake" on the real engines  
The engine module is a core, or "engine block" and it could mostly remain the same since it contains the core engine functionality which is common across most of the game engines (timers, game loop, memory management, and such things)  
But intakes could vary (turbochargers, superchargers, etc) and this is our case

By using the framework module it would be possible to:
* Implement and apply the 49 protocol for native implementation right away;
* Decouple the "goldsrc-specific" architecture from the engine core;
* It'll bring a lot more freedom to developers;
* It would be possible to use the engine core for own needs/projects;
* It will drastically improve both the extensibility and the modifiability of the engine;
* No crappy game loders would be needed;
* Support for both types of filesystems/gameui's (it won't be matter which type of implementation you want to use; you can even remove the usage of gameui and/or clientdll if you don't need them);

It's also possible to move the GameUI interaction code to client dll