# Contents:

* **common** - engine interfaces and common code;
* **dedicated** - dedicated server launcher;
* **engine** - engine module implementation (host + server functionality (+ game module handling));
* **engineclient** - engine client-side module (provides render, input, sound, gui functionality (and handles the client game module));
* **filesystem** - custom filesystem(_stdio) module implementation;
* **game** - contains (server) game and client game modules sources;
* **gameui** - Game User Interface module implementation;
* **input** - input system module;
* **launcher** - engine launcher code;
* **mathlib** - static math library;
* **network** - network system module;
* **optional** - engine optional components;
* **physics** - physics system module;
* **pm_shared** - reimplemented shared player movement code;
* **qlibc** - custom mini libc replacement;
* **render** - render module sources;
* **script** - provides scripting functionality for both client/server games;
* **sound** - sound system module (client-side only);
* **tier1** - custom tier1 static library sources;
* **utils** - engine tools sources;
* **vgui3** - new GUI system (custom implementation based on improved VGUI2 interfaces);
* **voice** - reimplemented voice codec module;