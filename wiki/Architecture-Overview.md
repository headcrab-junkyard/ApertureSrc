The key concept of the engine is that it oriented on server-role execution by default. That means that the core functionality contains only code required to simulate the game (server-side only) without the user interaction possibilities (= no client-side; no user inputs-outputs such as sound/graphics/input/gui)

All client-side functionality is connected to the engine by the "engineclient" module which is responsible for loading all client-specific modules, initializing and updating them (**UNSURE** and providing the client-side representation of the game world)

**TODO**
(try to rewrite the architecture for modern standards; apply x64 and mt support; highly modular; integrate new game interfaces; many new features with preservation of legacy api; would be based on lotta id software sources using the source engine interfaces (and hltv core sources?))