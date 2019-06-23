`* interface IWorld`

`* class CLocalWorld/CClientWorld : public IWorld`

`* class CGameWorld/CServerWorld : public IWorld`

```cpp
interface IWorld
{
    virtual void SpawnParticle(type, position, rotation, scale, etc) = 0;

    // ...
};

class CGameWorld : public IWorld
{
public:
    void SpawnParicle(...)
    {
        CSVCParticleMsg SVCParticleMsg(...);
        Server->SendMessageToAll(SVCParticleMsg);
    };
};

class CLocalWorld : public IWorld
{
public:
    void SpawnParticle(...)
    {
        // Actually spawn a visible particles entity
    };
};
```

**NOTES:**
* What's the purpose of using the IWorld interface? (CScene could use IWorld without knowing about client/server-specific parts)
* Client-specific code shouldn't be placed in server world and there should be two different instances of worlds because this way it's possible to way easily split the client code into its own library or remove it to use the rest of the codebase for dedicated server-only purposes and such things
* **World State Replication Example:** Server world -> send params about its state -> client-side net channel -> receive params -> local world -> accept them
* Client-side prediciton stuff should probably also be placed in local world code...