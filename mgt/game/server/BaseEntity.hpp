/// @file
/// @brief base game entity class

#pragma once

#include "game/server/IGameEntity.hpp"

typedef struct entvars_s entvars_t;

class CBaseEntity : public IGameEntity
{
public:
	CBaseEntity(entvars_t *vars) = default;
	virtual ~CBaseEntity() = default;
	
	virtual void OnSpawn() override {}
	virtual void OnTouch(const IEntity &apOther) override {}
	virtual void OnUse(const IEntity &apOther) override {}
	
	const char *GetClassName() const;
	float GetHealth() const;
protected:
	entvars_t *self{nullptr};
};