/// @file

#pragma once

#include "BaseEntity.hpp"

class CBaseItem;

class CBasePlayer : public CBaseEntity
{
public:
	CBasePlayer() = default;
	virtual ~CBasePlayer() = default;
	
	bool GiveItem(CGameItem *item, unsigned int giveFlags);
	void GiveItem(const char *name);
	
	void /*Start*/Reload();
	
	void ShowTip(const char *title, const char *tip, bool autoHide);
	void HideTip();
	
	bool IsTipVisible() const {return tipUp;}
	
	void OnSpawn() override;
	void OnThink() override;
	
	virtual	void OnTakeDamage(CBaseEntity *inflictor, CBaseEntity *attacker, const Vec3 &dir, const char *damageDefName, const float damageScale, const int location) override;
	
	void Die();
	
	virtual void OnKilled(CBaseEntity *inflictor, CBaseEntity *attacker, int damage, const Vec3 &dir, int location) override;
	
	edict_t *Drop_Item(gitem_t *item);
	void ChangeWeapon();
	void Think_Weapon(edict_t *ent);
private:
	// QC
	
	void DropQuad();
	void DropRing();
	void DropBackback();
	
	void GibPlayer();
	
	void DeathSound();
	
	float GetBestWeapon();
	bool CheckNoAmmo();
	
	void StopFiring();
	void FireWeapon();
	
	void FlashlightOn();
	void FlashlightOff();
	
	//CEntityPtr<CBaseWeapon> weapon;
	CBaseItem *weapon{nullptr];
	CBaseItem *lastweapon{nullptr];
};