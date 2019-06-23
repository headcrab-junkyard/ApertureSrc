# Magenta Engine
## Native Architecture Notes

Notes for native OOP engine architecture  
Can't always hold the future architecture thoughts in my head so I'll leave 'em here

* FileSystem - should be globally accessed but no global vars/singletons; pass it via constructors or create some kind of core class that will hold the logging, filesystem support and such. Access them from that class via Get* methods (GetLogger()->Log("Blah");)
* Renderer, sound, filesystem and other modules which impl was split into dll (it's not required for them to be split into dll, static linkage is possible but dynamic makes things a lot easier (mostly for recompilation time reduction). There is a high and low levels for them. High level is the engine impl - it mostly used for callbacking purposes (redirect call to low level) but it's also possible to add some common (impl independent) additional logic (logging and optional feature enablement like "-nosound" launch argument handling for the sound module). Low level is the actual implementation placed into it's own dll for simplicity and convenience.
* Keep [SOLID](https://en.wikipedia.org/wiki/SOLID_(object-oriented_design))
* [SOLID - Single Responsibility]: The class should have only one reason to be changed; For this codebase that means that (for example) high level filesystem should only interact with it's low level and provide some better interface from itself to client if it's needed. It shouldn't be responsible for loading the low level filesystem - the Dependency Injection is used here. We should load the fs module (create a null impl for tests or create it in case of static linkage) and pass it to the high level
* So from the previous point we need to find out the way of how the low level module implementations are going to be loaded - which level should be responsible for that?
* WTF is IGame interface and its impls? What's the purpose of that crappy structure in your code Valve? Is this thing should be used for client mode to open the window and init the graphics and ui and be skipped for dedicated mode? It doesn't look like that... (Potential problem: check that this doesn't want to use the fs or we need to change the init order from (init:game->engine|shutdown:engine->game) to reverse or init the fs at their level). I want to fully move the fs interaction to engine and simply provide it's factory from the interface. Then CEngine will contain some kind of filesystem loader class that will load the fs impl from the dll and provide it to the CFileSystem (high level fs)
* CFileSystem should be rewritten to use a new CFile class. It's Open/Close methods should now work with CFile instances, not FileHandle_t as in IFileSystem interface from V. This class is currently oriented only on internal engine module use but it will be really good to provide access to it via OGSAPI (IFile and IFileSystemOGS? interfaces presumably)
* Console print methods are calling the system print method. Is it good? There is a way it can be improved but should be?
* Maybe add some struct that will contain core subsystem pointers? Like:

```cpp
struct TCoreSubsystems
{
    IMemory *pMemory;
    ISystem *pSystem;
    IFileSystem *pFileSystem;
};
```

* Legacy event listeners were rewritten to use the CLegacyGame instead of direct exports funcs. Can't say it's a good nor bad idea for now. If the CLegacyGameLoader would be responsible for importing the game exports we can manually fix the funcs that was unexported (in HLSDK1 gamedll case) by providing the null funcs impl for them. For now I want to provide a default impl for both DLL_FUNCTIONS and NEW_DLL_FUNCSTIONS which will use null impl. They will be passed to gamedll and some of func pointers will be replaced with they're actual impl and the rest will stay untouched. Using this approach we don't need to check for func presence like that:

```cpp
if(gEntityInterface.pfnGameInit)
    gEntityInterface.pfnGameInit();
```
We can freely call an export and if it's not present in gamedll then it wouldn't be replaced in our func table and it's null impl will be called instead (I think that it'll be good to init the CLegacyGame with null-implemented exports and accept external impls by overriding our own. I think we need an additional Clear/Reset method that will reset the export funcs impl back to null)
* Legacy console command processing mode: Old ConCmds are registered using the func pointer that doesn't expect any args passed to it. New ConCmds are using a const IConCmdArgs reference that is used for the same purpose as the old Cmd_Args/Cmd_Argv/Cmd_Argc func set. Old ComCmds still be supported for backward-compat but the whole new system will be using the IConCmds interface for args passing
* Replace the Sys_Error func with CSysError exception?
* Merge CEngine with CHost
* Put console/cmdline/filesystem(?)/memory into system module and access em via getters
* Add CSystem as each other module dependency
* CSystem = high-level system module = set of core components that is used across the whole codebase (memory/console/debugging/logging/fs/etc). Has a low level impl ISystem that can be put into dll if you want
* The new architecture is finally coming out of the dark and now it's more clear how it will look like...

Subsystem init order:
* FileSystem (who is responsible for mounting it in hlds and in hl?)
* System - print info about hardware (cpu info, features, available memory and such)
* Console - need to split the drawing code from the logic in order to use it without the graphics subsystem support (headless mode that's used for dedicated server)
* Network
* Graphics (Video) (Sound for Linux; quake rules; dunno about gs way) - open the main window here? (IGame has a CreateMainWindow method but who calls it?) and load our renderer dll; current code is based on q2 so we have only one renderer (but we could write an impl will be able detect all the potentially workable render modules and create a list of them)
* GUI? (Should be inited somewhere around here)
* Sound (Video for Linux; quake rules; dunno about gs way) - load the sound module
* View
* Menu -> EngineGui? (GameUI, GameConsole)
* Screen
* CDAudio (why not in the sound init?)
* Input (move higher?)
* autoexec.cfg