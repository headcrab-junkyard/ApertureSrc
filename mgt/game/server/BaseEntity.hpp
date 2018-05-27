/// @file
/// @brief base entity class

#pragma once

typedef struct entvars_s entvars_t;

class CBaseEntity
{
public:
	CBaseEntity(entvars_t *vars) = default;
	virtual ~CBaseEntity() = default;
	
	virtual void OnSpawn(){}
	virtual void OnTouch(const CBaseEntity &apOther){}
	virtual void Use(const CBaseEntity &apOther){}
	
	void SetModel(const char *name);
	void SetSize(float *mins, float *maxs);
	
	const char *GetClassName() const;
	float GetHealth() const;
protected:
	entvars_t *self{nullptr};
};