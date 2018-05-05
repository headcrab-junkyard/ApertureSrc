/// @file

#pragma once

class CEntity
{
public:
	CEntity() = default;
	virtual ~CEntity() = default;
	
	virtual void OnSpawn(){}
	virtual void Use(const CEntity &apOther){}
protected:
	entvars_t *self{nullptr};
};