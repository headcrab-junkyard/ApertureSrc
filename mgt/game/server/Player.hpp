/// @file

#pragma once

#include "GameEntity.hpp"

class CPlayer : public CGameEntity
{
public:
	CPlayer() = default;
	virtual ~CPlayer() = default;
	
	void Die();
private:
	void DropQuad();
	void DropRing();
	void DropBackback();
	
	void GibPlayer();
	
	void DeathSound();
	
	float GetBestWeapon();
	bool CheckNoAmmo();
};