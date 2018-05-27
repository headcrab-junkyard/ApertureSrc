/// @file

#pragma once

#include "BaseEntity.hpp"

class CPlayer;

class CBaseWeapon : public CBaseEntity
{
public:
	CBaseWeapon() = default;
	virtual ~CBaseWeapon() = default;
	
	// Init
	
	virtual void Spawn() override; // OnSpawn
	virtual void Precache() override; // OnPrecache
	
	void SetOwner(CPlayer *apOwner){mpOwner = apOwner;}
	/*inline*/ CPlayer *GetOwner() const {return mpOwner;}
	
	// State control/player interface
	
	void PlayerPickup(CPlayer *apPlayer); // this will call the OnPlayerPickup
	
	void Idle();
	
	void Reload();
	
	void Lower();
	void Raise();
	
	void Hide();
	void Show();
	
	void Attack();
	
	bool CanDrop() const;
	
	bool IsReloading() const;
	
	enum class eWeaponID : int
	{
		Unknown = -1
	};
	
	eWeaponID GetID() const {return meID;}
protected:
	///< Override this method for your custom weapons
	virtual void OnPlayerPickup(){}
	
	virtual void OnReload(){}
	
	virtual void OnAttack(){}
	
	CBasePlayer *mpOwner{nullptr};
	
	// Ammo management
	ammo_t meAmmoType{};
	int mnAmmoRequired{0}; ///< Amount of ammo to use each shot. 0 means weapon doesn't need ammo
	int mnClipSize{0}; ///< 0 means no reload
	idPredictedValue<int> mAmmoClip;
	int mnLowAmmo{0}; ///< If ammo in clip hits this threshold, snd_
	//bool mbPowerAmmo{false}; ///< true if the clip reduction is a factor of the power setting when a projectile is launched
	
	eWeaponID meID{eWeaponID::Unknown};
};