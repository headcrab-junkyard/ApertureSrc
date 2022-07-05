# Contents:

* **common** - engine interfaces and common code;
* **engine** - engine module implementation (host + server-side functionality (+ game module handling));
* **engineclient** - engine client-side module (provides renderer, input, sound, gui functionality (and handles the client game and game ui modules));
* **filesystem** - custom filesystem(_stdio) module implementation;
* **inputsystem** - input system module;
* **networksystem** - network system module;
* **optional** - engine optional components;
* **physics** - physics system module;
* **qlibc** - custom mini libc replacement;
* **renderer** - render module sources;
* **scriptsystem** - provides scripting functionality for both client/server games;
* **soundsystem** - sound system module (client-side only);
* **tier1** - custom tier1 static library sources;
* **vgui3** - new GUI system (custom implementation based on improved VGUI2 interfaces);
* **voice** - reimplemented voice codec module;