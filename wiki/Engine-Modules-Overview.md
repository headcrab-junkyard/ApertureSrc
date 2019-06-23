* **input** - collects the input data from the input devices and translates it into the input events;
* **network** - all network-related routines; should be responsible for client-server networking; (UDP) packet sending; probably file sending/receiving;
* **physics** - movement simulation; collision detection; ray-tracing and such things;
* **sound** - sound playing; (probably also should be responsible for sound loading)
* **graphics/render** - rendering stuff; (probably should be responsible for texture and model loading)
* **system**(?) - mostly used as an abstraction above the OS (it's platform-dependent and that's the reason it's currently split); this module should contain all the fundamental things like timers, memory managing and that kind of stuff; (will be better to merge it with the engine module; in fact current version the engine module contains some of the system module logic)
* **engine/core** - (timers/threads/gameloop/module connection) main (core) module of the engine that provides a connection between the other modules; responsible for gameui/clientdll/gamedll managing (loading/unloading);
* **gameui** - client-side menu system;
* **clientdll** - client-side game logic;
* **gamedll** - server-side game logic;
* **filesystem** - virtual file system module (probably should be placed in the engine module);
* **framework** - engine<->game code connection layer (some kind of framework module for genre-specific stuff? in that case engine module should be renamed to core and contain only base stuff without any piece of game-specific logic);
* **voice** - voip stuff;
* **engineclient**(?) - engine client-side functionality;
* **script**(?) - scripting engine;