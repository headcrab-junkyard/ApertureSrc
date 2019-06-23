Dedicated server mode (default):
* LauncherApp->LauncherDLL(Dedicated)->Load core module (and base submodules)->Init->loop(Run)->Exit

Client mode:
* LauncherApp->LauncherDLL(Client)->Load core module (and base submodules) and client modules->Init->loop(Run client side->Run core side)->Exit

Client mode is enabled when not in dedicated server mode  
In client mode we also load all the user interaction modules (graphics, input, sound, ui and such)  
So on code level this should load some additional external libs containing the client-side modules (or it should call the integrated functions in case of using the static libs)  
Client-side should be decoupled from the server-side but it's possible for both client/server sides to be connected with the common/host part  
In case of dedicated server no one of these client modules will be initialized and updated later so it won't be any additional resource consumptions for dedicated server  
It's possible to use the own dll for whole client-side logic but makes not much sense from this point

## Frame on client side
* Window message pump (Win32)
* Poll input (and convert to concmds)
* Core: process concmds
* Core: Network frame
* Core: Physics frame
* Core: Game frame
* Core: plugins frame
* Draw scene (sync with physworld)
* Draw gui elements
* GameUI frame
* ClientDLL frame
* Swap buffers