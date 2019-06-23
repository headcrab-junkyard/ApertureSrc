* Lang: **C++** (C++14/C++17 standards)
* OOP/COP
* Modularity
* Game and engine plugins
* It should use one interface for both user and dedicated server modes
* **Headless mode** should be implemented via additional loadable modules (graphics, input, sound) for user mode - there is no point to have all this functionality in core module which wouldn't use it in for dedicated server
* **-dedicated** or **-headless** command line arg
* Some kind of **shell module for user mode**?