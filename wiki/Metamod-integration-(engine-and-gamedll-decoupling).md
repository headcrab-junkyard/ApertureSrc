# Engine metamod module integration draft

* Add special class for game dll (CGameHandler)
* Call all gamedll funcs through it

```cpp
void CGameHandler::Frame()
{
    pMetamod->Frame(); // call plugins preframe -> gamedll frame -> plugins postframe
};
```

Instead of direct gamedll function calling, we should call it via some intermediate level mechanism

This approach will allow to:
* Integrate the engine with metamod;
* Call legacy api game dll from the new arch;
* (Something else?)

Engine -> game protocol -> gamedll/metamod plugins/new game dll/whatever you want, it mostly just an event...