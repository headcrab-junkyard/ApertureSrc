/// @file

#include "engine/IEntity.hpp"

class CEntity final : public IEntity
{
public:
	CEntity(edict_t *apEdict);
	~CEntity();
	
	void SetGameEntity(IGameEntity *apGameEntity) override;
	IGameEntity *GetGameEntity() const override {return mpGameEntity;}
	
	void SetOrigin(float *org) override;
	void SetSize(float *mins, float *maxs) override;
	void SetModel(const char *asName) override;
private:
	void SetMinMaxSize(float *min, float *max, qboolean rotate);
	
	edict_t *e{nullptr};
	IGameEntity *mpGameEntity{nullptr};
};