/// @file

#pragma once

class CPlayer;

class CWeapon
{
public:
	CWeapon() = default;
	virtual ~CWeapon() = default;
	
	// Init
	
	void Spawn();
	
	void SetOwner(CPlayer *apOwner){mpOwner = apOwner;}
	/*inline*/ CPlayer *GetOwner() const {return mpOwner;}
	
	// State control/player interface
	
	void Reload();
	
	void Lower();
	void Raise();
	
	void Hide();
	void Show();
	
	void Fire();
	
	bool CanDrop() const;
	
	bool IsReloading() const;
private:
	CPlayer *mpOwner{nullptr};
	
	// Ammo management
	ammo_t meAmmoType{};
	int mnAmmoRequired{0}; ///< Amount of ammo to use each shot. 0 means weapon doesn't need ammo
	int mnClipSize{0}; ///< 0 means no reload
	idPredictedValue<int> mAmmoClip;
	int mnLowAmmo{0}; ///< If ammo in clip hits this threshold, snd_
	//bool mbPowerAmmo{false}; ///< true if the clip reduction is a factor of the power setting when a projectile is launched
};